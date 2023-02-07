#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <type_traits>
#include <cassert>

namespace HWMatrix
{

template<typename T>
class MyUniquePtr final
{
  T* buf_;

  public:
  // Ctor
  MyUniquePtr(size_t sz = 0):
  buf_{(sz == 0) ? nullptr : new T[sz]} { }

  MyUniquePtr(const MyUniquePtr& rhs) = delete;
  MyUniquePtr& operator=(const MyUniquePtr& rhs) = delete;

  MyUniquePtr(MyUniquePtr&& rhs_) noexcept 
  {
    buf_ = rhs_.buf_;
    rhs_.buf_ = nullptr;
  }

  MyUniquePtr& operator= (MyUniquePtr&& rhs_) noexcept
  {
    if (this != &rhs_)
    {
      T* tmp = rhs_.buf_;
      rhs_.buf_ = buf_;
      buf_ = tmp;
    }
    return *this;
  }

  ~MyUniquePtr()
  {
    delete[] buf_;
  }

  T* get() const noexcept { return buf_; }
};

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
  };

  private:
  MyUniquePtr<T> data_;         // Buffer that stores all the matrix content
  MyUniquePtr<RowProxy> rows_;  // Array of Proxys that point to beginning of each row inside the data_
  size_t n_;  // Dimensions
  size_t m_;  //

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

  // Copies content from another type matrix
  // Is not except-safe, but used in an except-safe way in copy() and copy ctor
  // Better implementation uses iterators and std::copy, but I am not confident
  // that I properly know the details of implementing custom iterator
  template<typename CopyT>
  void copy_from(const Matrix<CopyT>& other)
  {
    assert(other.dims().n == n_); // This function is private so
    assert(other.dims().m == m_); // We assume that it is called correctly
    
    for (int i = 0; i < n_; ++i)
    {
      for(int k = 0; k < m_; ++k)
        (*this)[i][k] = other[i][k];
    }
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
    data_{n * m},
    rows_{n}
  {
    if((n == 0) || (m == 0))
      throw std::invalid_argument("Attempt to create an object with incorrect dimensions");

    // Orders rows in straight order
    for (size_t row = 0; row < n_; ++row)
    {
      (*this)[row] = {data_.get(), row * m_, m_};
    }
  }

  // Constructs empty matrix and fills it with values in row-major order
  Matrix(size_t n, size_t m, const std::vector<T>& values): Matrix(n, m)
  {
    if (values.size() != size())
      throw std::invalid_argument("Vector size does not fit the matrix dimensions");;
    
    std::copy(values.begin(), values.end(), data_.get());
  }
  
  // Copy ctor
  Matrix(const Matrix& other): Matrix(other.n_, other.m_)
  {
    copy_from(other);
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

  // Copy assign from another type matrix
  template<typename CopyT>
  Matrix& operator=(const Matrix<CopyT>& src)
  {
    Matrix newm(src);
    std::swap(*this, newm);
    return *this;
  }

  // Copy ctor from another type matrix
  template<typename CopyMat>
  Matrix(const CopyMat& src): Matrix(src.dims().n, src.dims().m)
  {
    copy_from(src);
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
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat)
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