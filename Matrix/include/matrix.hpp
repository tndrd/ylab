#pragma once

#include <iostream>
#include <memory>
#include <vector>

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

template<typename T, size_t n, size_t m>
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
    T* buf_;

    public:
    RowProxy(T* parent_buf=nullptr, size_t ofs=0) noexcept: ofs_(ofs), buf_(parent_buf) {}

    const T&  operator[] (size_t k) const &
    {
      if(k >= m) throw;
      return *(buf_ + ofs_ + k);
    }

    T& operator[] (size_t k) &
    {
      return (*this)[k];
    }
    
    T&& operator[] (size_t k) &&
    {
      return std::move((*this)[k]);
    }

    // Switches to another buffer without losing its order in row array
    void change_parent(Matrix& new_parent) noexcept
    {
      buf_ = new_parent.data_.get();
    }
  };

  std::unique_ptr<T> data_;         // Buffer that stores all the matrice content
  std::unique_ptr<RowProxy> rows_;  // Array of Proxys that point to beginning of each row inside the data_

  // Fills rows so first row proxy point to first row in data_ and so on 
  void order_rows() noexcept
  {
    for (size_t row = 0; row < n; ++row)
    {
      (*this)[row] = {(*this).data_.get(), row * m};
    }
  }

  // Remaps rowproxys to current data_ buffer
  void remap_rows() noexcept
  {
    for (size_t row = 0; row < n; ++row)
    {
      (*this)[row].change_parent(*this);
    }
  }

  // Deep copies all the content and all the proxies from other matrix
  void deepcopy_from(const Matrix& other)
  {
    std::copy(other.data_.get(), other.data_.get() + size(), data_.get());
    std::copy(other.rows_.get(), other.rows_.get() + n, rows_.get());
  }

  public:

  struct Dim
  {
    size_t n_;
    size_t m_;
  };

  size_t size() const noexcept {return n * m;}
  Dim    dims() const noexcept {return {n, m};}

  // Constructs empty matrix
  Matrix(): data_{new T[size()]}, rows_{new RowProxy[n]}
  {
    DEBUG_PRINT("Matrix ctor");
    order_rows();
  }

  // Constructs empty matrix and fills it with values in row-major order
  Matrix(std::vector<T>& values): Matrix()
  {
    if (values.size() != size()) throw;
    std::copy(values.begin(), values.end(), data_.get());
  }

  ~Matrix()
  {
    DEBUG_PRINT("Matrix dtor");
  }

  // Copy constructor. Deep copies all the data and then remaps recieved RowProxys to current buffer. This way it saves the original row order.
  // Question: how to avoid calling base constructor here? I mean, is there a way to construct objects inside std::copy with their copy ctor?
  //                                      |
  //                                      ⌄
  Matrix(const Matrix& other): data_{new T[size()]}, rows_{new RowProxy[n]}
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

    //delete[] rows_;         // No need for that
    //delete[] data_;         // sizeof(T), n , m are equal for both matrices,
                              // so we do not need to allocate new buffers.
    //data_{new T[size()]};   // We can simply rewrite the old ones    
    //rows_{new RowProxy[n]}; // Again, remap_rows saves the original row order

    deepcopy_from(other);
    remap_rows();
    return *this;
  }

  // Move ctor
  Matrix(Matrix&& other) noexcept: data_(std::move(other.data_)), rows_(std::move(other.rows_))
  {
    DEBUG_PRINT("Matrix move ctor");
  }

  // Move assignment
  Matrix& operator= (Matrix&& other) noexcept
  {
    DEBUG_PRINT("Matrix move=");

    if (this == &other)
      return *this;

    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);

    return *this;
  }

  // Returns row proxy lvalue so the row can be acessed
  const RowProxy& operator[] (size_t i) const &
  {
    if(i >= n) throw;
    return rows_.get()[i];
  }
  
  RowProxy& operator[] (size_t i) &
  {
    return (*this)[i];
  }

  // Return row proxy rvalue if needed
  RowProxy&& operator[] (size_t i) &&
  {
    return std::move((*this)[i]);
  }

  // Question: do i need
  // const RowProxy&& operator[] (size_t i) const &&
  // And therefore const T&& RowProxy::operator[] const && ?

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
};

template<typename T, size_t n>
double det(const Matrix<T, n, n>& mat)
{
  Matrix<T,n,n> copy {mat};

  size_t n_swaps = 0;

  for (size_t col = 0; col < n; ++col)
  {
    bool found = false; 
    size_t row = col;

    for(; row < n; ++row)
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

    for (size_t lower_row = col + 1; lower_row < n; ++lower_row)
    {
      T coeff = copy[lower_row][col] / copy[col][col]; 
      for(int k = col; k < n; ++k)
      {
        copy[lower_row][k] -= coeff * copy[col][k];
      }
    }
  }

  double determinant = 1 - static_cast<int>(2 * (n_swaps % 2));

  for(size_t i = 0; i < n; ++i)
  {
    determinant *= copy[i][i];
  }
  return determinant;
}

// Just a dump
template<typename T, size_t n, size_t m>
std::ostream& operator<<(std::ostream& os, Matrix<T, n, m>& mat)
{
  for (size_t i = 0; i < n; ++i)
  {
    os << "{ ";
    for (size_t k = 0; k < m; ++k)
    {
      os << mat[i][k] << " ";
    }
    os << "}" << std::endl;
  }
  return os;
}

}