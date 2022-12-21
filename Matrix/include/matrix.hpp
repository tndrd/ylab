#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <type_traits>

namespace HWMatrix
{

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

  // Stored type must be default constructible to create empty matrix
  static_assert(std::is_default_constructible<T>::value, "Type must be default constructible");

  // Class that stores row offset ofs_ in data buffer buf_
  class RowProxy final
  {
    size_t ofs_;
    size_t sz_;
    T* buf_;

    T& at(size_t k) const
    {
      if(k >= sz_) throw std::out_of_range("Column index out of range");
      return *(buf_ + ofs_ + k);
    }

    public:
    RowProxy(T* parent_buf=nullptr, size_t ofs=0, size_t sz=0) noexcept:
      ofs_{ofs}, buf_{parent_buf}, sz_{sz} {}

    T& operator[] (size_t k) &
    {
      return at(k);
    }

    const T& operator[] (size_t k) const &
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
  std::unique_ptr<T[]> data_;         // Buffer that stores all the matrix content
  std::unique_ptr<RowProxy[]> rows_;  // Array of Proxys that point to beginning of each row inside the data_
  size_t n_;  // Dimensions
  size_t m_;  //
  
  // Fills rows so first row proxy points to first row in data_ and so on 
  // Disclaimer: I know that this method is not except-safe and copy&swap trick may be used here, but I don't want to
  // use it and create excessive copies due to privacy that grants that this method will be used in an except-safe way
  void order_rows()
  {
    for (size_t row = 0; row < n_; ++row)
    {
      (*this)[row] = {data_.get(), row * m_, m_};
    }
  }

  // Remaps rowproxys to current data_buffer
  // Disclaimer: I know that this method is not except-safe and copy&swap trick may be used here, but I don't want to
  // use it and create excessive copies due to privacy that grants that this method will be used in an except-safe way
  void remap_rows()
  {
    for (size_t row = 0; row < n_; ++row)
    {
      (*this)[row].change_parent(*this);
    }
  }

  // Deep copies all the content and all the proxies from other matrix
  // Disclaimer: I know that this method is not except-safe and copy&swap trick may be used here, but I don't want to
  // use it and create excessive copies due to privacy that grants that this method will be used in an except-safe way
  void deepcopy_from(const Matrix& other)
  {
    if ((other.n_ != n_) || (other.m_ != m_))
      throw std::invalid_argument("Attempt to deepcopy from Matrix with another size");
    
    std::copy(other.data_.get(), other.data_.get() + size(), data_.get());
    std::copy(other.rows_.get(), other.rows_.get() + n_,     rows_.get());
  }

  // Fills the matrix with values in row-major order
  // Disclaimer: I know that this method is not except-safe and copy&swap trick may be used here, but I don't want to
  // use it and create excessive copies due to privacy that grants that this method will be used in an except-safe way
  void read_from(const std::vector<T>& values)
  {
    if (values.size() != size())
      throw std::invalid_argument("Vector size does not fit the matrix dimensions");;
    
    std::copy(values.begin(), values.end(), data_.get());
  }

  // Returns row proxy lvalue ref so the row can be acessed
  RowProxy& at(size_t i) const
  {
    if(i >= n_) throw std::out_of_range("Row index out of range");
    return rows_.get()[i];
  }

  // And here's the main feature of this complicated matrix:
  // O(1) row swap!
  Matrix& basic_swap_rows(size_t r1, size_t r2) &
  {
    std::swap((*this)[r1], (*this)[r2]);
    return *this;
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
  Matrix(size_t n, size_t m):
    n_(n),
    m_(m),
    data_{std::make_unique<T[]>(n * m)},
    rows_{std::make_unique<RowProxy[]>(n)}
  {
    if((n == 0) || (m == 0))
      throw std::invalid_argument("Attempt to create an object with incorrect dimensions");

    order_rows();
  }

  // Constructs empty matrix and fills it with values in row-major order
  Matrix(size_t n, size_t m, const std::vector<T>& values): Matrix(n, m)
  {
    read_from(values);
  }

  // Copy constructor. Deep copies all the data and then remaps recieved RowProxys to current buffer. This way it saves the original row order.
  // Future feature: make some magic with new to avoid calling default constructor for each item.
  Matrix(const Matrix& other): Matrix(other.n_, other.m_)
  {
    deepcopy_from(other);
    remap_rows();
  }

  // Copy assignment
  // Exception-safe, copy&swap trick is used
  Matrix& operator= (const Matrix& other)
  {
    if (this == &other)
      return *this;

    Matrix tmp {other};
    std::swap(*this, tmp);

    return *this;
  }

  // Move ctor & assignment
  Matrix(Matrix&& other) noexcept = default;
  Matrix& operator= (Matrix&& other) noexcept = default;

  // Virtual dtor
  virtual ~Matrix() {}

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

  // Returns row proxy rvalue ref if called by rvalue matrix
  RowProxy&& operator[] (size_t i) &&
  {
    return std::move(at(i));
  }

  // lvalue Matrix rowswap
  Matrix& swap_rows(size_t r1, size_t r2) &
  {
    return basic_swap_rows(r1, r2);
  }

  // rvalue Matrix rowswap
  Matrix&& swap_rows(size_t r1, size_t r2) &&
  {
    return std::move(basic_swap_rows(r1, r2));
  }
};

// Just a dump
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat)
{
  size_t n = mat.dims().n;
  size_t m = mat.dims().m;

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