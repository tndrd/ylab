#pragma once

#include "matrix.hpp"
#include <exception>
#include <cmath>
#include <algorithm>

namespace HWMatrix
{

inline const double FIT_TOLERANCE = 0.0001;

inline bool fit(double a, double b) noexcept
{
  return std::abs(a - b) < FIT_TOLERANCE;
}

template<typename T>
inline double ge_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  Matrix<T> copy {mat};
  size_t n = copy.dims().n;
  size_t swaps = 0;
  double determinant = 1;
  
  for (size_t col = 0; col < n; ++col)
  {
    bool found = false; 
    size_t row = col;

    for(; row < n; ++row)
    {
      if (!fit(copy[row][col], 0))
      {
        found = true;
        break;
      }
    }
    if (!found) return 0;

    copy.swap_rows(col, row);
    swaps += !!(col - row);
    determinant *= copy[col][col];
    
    for (size_t lower_row = col + 1; lower_row < n; ++lower_row)
    {
      T coeff = copy[lower_row][col] / copy[col][col]; 
      for(int k = col; k < n; ++k)
      {
        copy[lower_row][k] -= coeff * copy[col][k];
      }
    }
  }

  determinant *= 1 - static_cast<int>(2 * (swaps % 2));
  return determinant;
}

template<typename T>
inline Matrix<T> identity_matrix(size_t n)
{
  Matrix<T> idmt(n, n);

  for (int i = 0; i < n; ++i)
    idmt[i][i] = 1;
  
  return idmt;
}

template<typename T>
inline double lu_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  size_t n = mat.dims().n;

  Matrix<T> L = identity_matrix<T>(n); 
  Matrix<T> U {n, n};

  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      if (i <= j)
      {
        T val = mat[i][j];
        for (int k = 0; k < i; ++k)
          val -= L[i][k] * U[k][j];

        if (i == j && fit(val, 0)) return 0;
        U[i][j] = val;
      }
      else
      {
        T val = mat[i][j];
        for (int k = 0; k < j; ++k)
          val -= L[i][k] * U[k][j];

        L[i][j] = val / U[j][j];
      }
    }
  }

  double determinant = 1;

  for (int i = 0; i < n; ++i)
  {
    determinant *= U[i][i];
  }

  return determinant;
}

template<typename T>
inline double qr_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  size_t n = mat.dims().n;

  Matrix<T> a {mat};
  Matrix<T> q {n, n}; 
  Matrix<T> r {n, n};

  for (int k = 0; k < n; ++k)
  {
    T s = 0;
    for (int j = 0; j < n; ++j) s += a[j][k] * a[j][k];
    r[k][k] = std::sqrt(s);

    for (int j = 0; j < n; ++j) q[j][k] = a[j][k] / r[k][k];

    for (int i = k + 1; i < n; ++i)
    {
      T s = 0;
      for (int j = 0; j < n; ++j) s += a[j][i] * q[j][k];
      r[k][i] = s;

      for (int j = 0; j < n; ++j) a[j][i] = a[j][i] - r[k][i] * q[j][k];
    }
  }
  double determinant = 1 - 2*std::signbit(ge_det(q));

  for (int i = 0; i < n; ++i)
  {
    determinant *= r[i][i];
  }

  return determinant;
}

template<typename T>
inline double det(const Matrix<T>& mat)
{
  return ge_det(mat);
}

}