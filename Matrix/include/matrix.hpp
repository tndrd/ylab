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

  class RowProxy
  {
    size_t  ofs_;
    T* buf_;

    public:
    RowProxy(T* parent_buf=nullptr, size_t ofs=0) noexcept: ofs_(ofs), buf_(parent_buf) {}

    T&  operator[] (size_t k) const &
    {
      DEBUG_PRINT("T& RowProxy::operator[] &");
      if(k >= m) throw;
      return *(buf_ + ofs_ + k);
    }
    
    T&& operator[] (size_t k) const &&
    {
      DEBUG_PRINT("moving |");
      return std::move((*this)[k]);
    }

    void change_parent(Matrix& new_parent) noexcept
    {
      buf_ = new_parent.data_.get();
    }
  };

  std::unique_ptr<T> data_;
  std::unique_ptr<RowProxy> rows_;

  void order_rows() noexcept
  {
    for (size_t row = 0; row < n; ++row)
    {
      (*this)[row] = {(*this).data_.get(), row * m};
    }
  }

  void adopt_rows() noexcept
  {
    for (size_t row = 0; row < n; ++row)
    {
      (*this)[row].change_parent(*this);
    }
  }

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

  Matrix(): data_{new T[size()]}, rows_{new RowProxy[n]}
  {
    DEBUG_PRINT("Matrix ctor");
    order_rows();
  }

  Matrix(std::vector<T>& values): Matrix()
  {
    if (values.size() != size()) throw;
    std::copy(values.begin(), values.end(), data_.get());
  }

  ~Matrix()
  {
    DEBUG_PRINT("Matrix dtor");
  }

  // Question: how to avoid calling base constructor here? I mean, is there a way to construct objects inside std::copy with their copy ctor?
  //                                      |
  //                                      ⌄
  Matrix(const Matrix& other): data_{new T[size()]}, rows_{new RowProxy[n]}
  {
    DEBUG_PRINT("Matrix copy ctor");
    deepcopy_from(other);
    adopt_rows();
  }

  Matrix& operator= (const Matrix& other)
  {
    DEBUG_PRINT("Matrix copy=");

    if (this == &other)
      return *this;

    //delete[] rows_;         // No need for that
    //delete[] data_;         // Dims are equal, so we do not need to allocate new buffer
    //data_{new T[size()]};   // We can simply rewrite the old one
    
    //rows_{new RowProxy[n]}; // Also there is no need to allocate new RowProxys 
                              // Because size_t(T), n , m are equal for both matrices

    deepcopy_from(other);
    adopt_rows();

    return *this;
  }

  Matrix(Matrix&& other) noexcept: data_(std::move(other.data_)), rows_(std::move(other.rows_))
  {
    DEBUG_PRINT("Matrix move ctor");
    adopt_rows();
  }

  Matrix& operator= (Matrix&& other) noexcept
  {
    DEBUG_PRINT("Matrix move=");

    if (this == &other)
      return *this;

    data_ = std::move(other.data_);
    rows_ = std::move(other.rows_);
    adopt_rows();

    return *this;
  }

  RowProxy&  operator[] (size_t i) const &
  {
    DEBUG_PRINT("RowProxy& Matrix::operator[] &");
    if(i >= n) throw;
    return rows_.get()[i];
  }
  
  RowProxy&& operator[] (size_t i) const &&
  {
    DEBUG_PRINT("moving |");
    return std::move((*this)[i]);
  }

  Matrix& swap_rows(size_t r1, size_t r2) &
  {
    std::swap((*this)[r1], (*this)[r2]);
    return *this;
  }

  Matrix&& swap_rows(size_t r1, size_t r2) &&
  {
    return std::move(swap_rows(r1, r2));
  }
};

template<typename T, size_t n, size_t m>
inline std::ostream& operator<<(std::ostream& os, Matrix<T, n, m>& mat)
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