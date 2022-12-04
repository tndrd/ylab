#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

namespace HWMatrix
{

//#define DEBUG

#ifdef DEBUG
inline void DEBUG_PRINT(const char* str)
{
  std::cout << str << std::endl;
}
#endif

#ifndef DEBUG
inline void DEBUG_PRINT(const char* str) {}
#endif

template<typename T>
class Matrix
{
  // Brief: Matrix class with O(1) row swap possibility
  // All the matrix content is stored in one buffer [data_]
  // Single buffer grants access locality
  // Also there is a RowProxy array [rows_]. Each row proxy points to specific row in buffer
  // Right after the creation of matrix rows_[i][k] points to data_[i][k]
  // If we swap rows_[i] and rows_[j], rows_[i][k] will point to data_[j][k]
  // Wow! Rows i and j are now logically swapped (But physically not)!

  // The next important part is copying/moving
  // We want to save the original row order after copying/moving
  // So we need to inherit the original row order by remapping copied/moved
  // rowproxys to current data buffer. remap_rows() does it.  

  // Class that stores row offset ofs_ in data buffer buf_
  class RowProxy
  {
    size_t  ofs_;
    size_t sz_;
    T* buf_;

    T& at(size_t k) const
    {
      if(k >= sz_) throw;
      return *(buf_ + ofs_ + k);
    }

    public:
    RowProxy(T* parent_buf=nullptr, size_t ofs=0, size_t sz=0) noexcept: ofs_(ofs), buf_(parent_buf), sz_(sz) {}

    T&  operator[] (size_t k) &
    {
      return at(k);
    }

    const T&  operator[] (size_t k) const &
    {
      return at(k);
    }

    T&& operator[] (size_t k) &&
    {
      return std::move(at(k));
    }

    // Switches to another buffer without losing its order in row array
    void change_parent(Matrix& new_parent) noexcept
    {
      buf_ = new_parent.data_.get();
    }
  };
  
  private:
  std::unique_ptr<T> data_;         // Buffer that stores all the matrice content
  std::unique_ptr<RowProxy> rows_;  // Array of Proxys that point to beginning of each row inside the data_
  size_t n_;  // Dimensions
  size_t m_;  //
  
  // Fills rows so first row proxy point to first row in data_ and so on 
  void order_rows()
  {
    for (size_t row = 0; row < n_; ++row)
    {
      (*this)[row] = {(*this).data_.get(), row * m_, m_};
    }
  }

  // Remaps rowproxys to current data_ buffer
  void remap_rows()
  {
    for (size_t row = 0; row < n_; ++row)
    {
      (*this)[row].change_parent(*this);
    }
  }

  // Deep copies all the content and all the proxies from other matrix
  void deepcopy_from(const Matrix& other)
  {
    assert((other.n_ == n_) && (other.m_ == m_));
    std::copy(other.data_.get(), other.data_.get() + size(), data_.get());
    std::copy(other.rows_.get(), other.rows_.get() + n_, rows_.get());
  }

  // Returns row proxy lvalue ref so the row can be acessed
  RowProxy& at(size_t i) const
  {
    if(i >= n_) throw;
    return rows_.get()[i];
  }

  public:

  struct Dim
  {
    size_t n;
    size_t m;
  };

  size_t size()   const noexcept {return n_ * m_;}
  Dim    dims()   const noexcept {return {n_, m_};}
  bool is_sqare() const noexcept {return n_ == m_;}

  // Constructs empty matrix
  Matrix(size_t n, size_t m): n_(n), m_(m), data_{new T[n*m]}, rows_{new RowProxy[n]}
  {
    if((n == 0) || (m == 0)) throw;

    DEBUG_PRINT("Matrix ctor");
    order_rows();
  }

  // Constructs empty matrix and fills it with values in row-major order
  Matrix(size_t n, size_t m, std::vector<T>& values): Matrix(n, m)
  {
    if (values.size() != size()) throw;
    std::copy(values.begin(), values.end(), data_.get());
  }

  ~Matrix()
  {
    DEBUG_PRINT("Matrix dtor");
  }

  // Copy constructor. Deep copies all the data and then remaps recieved RowProxys to current buffer. This way it saves the original row order.
  // Question: how to avoid calling default constructor here? I mean, is there a way to construct objects inside std::copy with their copy ctor?
  //                                                                      |
  //                                                                      ⌄
  Matrix(const Matrix& other): n_(other.n_), m_(other.m_), data_{new T[other.size()]}, rows_{new RowProxy[other.n_]}
  {
    DEBUG_PRINT("Matrix copy ctor");
    deepcopy_from(other);
    remap_rows();
  }

  // Copy assignment
  Matrix& operator= (const Matrix& other)
  {
    DEBUG_PRINT("Matrix copy=");

    if (this == &other)
      return *this;

    if((n_ != other.n_) || (m_ != other.m_)) throw;

    //delete[] rows_;         // No need for that
    //delete[] data_;         // sizeof(T), n_ , m_ are equal for both matrices,
                              // so we do not need to allocate new buffers.
    //data_{new T[size()]};   // We can simply rewrite the old ones    
    //rows_{new RowProxy[n_]}; // Again, remap_rows saves the original row order

    deepcopy_from(other);
    remap_rows();
    return *this;
  }

  // Move ctor
  Matrix(Matrix&& other) noexcept: n_(other.n_), m_(other.m_), data_(std::move(other.data_)), rows_(std::move(other.rows_))
  {
    DEBUG_PRINT("Matrix move ctor");
  }

  // Move assignment
  Matrix& operator= (Matrix&& other) noexcept
  {
    DEBUG_PRINT("Matrix move=");

    if (this == &other)
      return *this;
    
    std::swap(n_, other.n_);
    std::swap(m_, other.m_);
    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);

    return *this;
  }

  // Returns lvalue ref if called by non-const matrix
  RowProxy& operator[] (size_t i) &
  {
    return at(i);
  }

  // Returns const lvalue ref if called by const matrix
  const RowProxy& operator[] (size_t i) const &
  {
    return at(i);
  }

  // Returns row proxy rvalue rvalue if called by rvalue matrix
  RowProxy&& operator[] (size_t i) &&
  {
    return std::move(at(i));
  }

  // And here's the main feature of this complicated matrix:
  // O(1) row swap!
  Matrix& swap_rows(size_t r1, size_t r2) &
  {
    std::swap((*this)[r1], (*this)[r2]);
    return *this;
  }

  // The same but with rvalue matrix
  Matrix&& swap_rows(size_t r1, size_t r2) &&
  {
    return std::move(swap_rows(r1, r2));
  }

  std::ostream& dump(std::ostream& os) const
  {
    for (size_t i = 0; i < n_; ++i)
    {
      os << "{ ";
      for (size_t k = 0; k < m_; ++k)
      {
        os << (*this)[i][k] << " ";
      }
      os << "}" << std::endl;
    }

    return os;
  }
};

template<typename T>
double det(const Matrix<T>& mat)
{
  if(!mat.is_sqare()) throw;

  size_t n_ = mat.dims().n;

  Matrix<T> copy {mat};

  size_t n_swaps = 0;

  for (size_t col = 0; col < n_; ++col)
  {
    bool found = false; 
    size_t row = col;

    for(; row < n_; ++row)
    {
      if (copy[row][col] != 0)
      {
        found = true;
        break;
      }
    }

    if (!found) {return 0;}

    copy.swap_rows(col, row);
    n_swaps += !!(col - row);

    for (size_t lower_row = col + 1; lower_row < n_; ++lower_row)
    {
      T coeff = copy[lower_row][col] / copy[col][col]; 
      for(int k = col; k < n_; ++k)
      {
        copy[lower_row][k] -= coeff * copy[col][k];
      }
    }
  }

  double determinant = 1 - static_cast<int>(2 * (n_swaps % 2));

  for(size_t i = 0; i < n_; ++i)
  {
    determinant *= copy[i][i];
  }
  return determinant;
}

// Just a dump
template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat)
{
  return mat.dump(os);
}

}