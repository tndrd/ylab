#pragma once

#include "matrix.hpp"
#include <exception>
#include <cmath>
#include <algorithm>

namespace HWMatrix
{

static const double FIT_TOLERANCE = 0.00001;
static bool fit(double a, double b) noexcept
{
  return std::abs(a - b) < FIT_TOLERANCE;
}

template<typename T, typename EvalT=double>
Matrix<EvalT> eval_matrix(const Matrix<T>& mat)
{
  size_t n = mat.dims().n;
  Matrix<EvalT> eval {n, n};

  for (int i = 0; i < n; ++i)
  {
    for(int k = 0; k < n; ++k)
      eval[i][k] = mat[i][k];
  }

  return eval;
}

template<typename T, typename EvalT=double>
EvalT ge_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  Matrix<EvalT> copy = eval_matrix(mat);
  size_t n = mat.dims().n;

  size_t swaps = 0;
  EvalT determinant = 1;
  
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
      EvalT coeff = copy[lower_row][col] / copy[col][col]; 
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
Matrix<T> identity_matrix(size_t n)
{
  Matrix<T> idmt(n, n);

  for (int i = 0; i < n; ++i)
    idmt[i][i] = 1;
  
  return idmt;
}

template<typename T, typename EvalT=double>
EvalT lu_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  size_t n = mat.dims().n;

  Matrix<EvalT> L = identity_matrix<EvalT>(n); 
  Matrix<EvalT> U {n, n};

  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      if (i <= j)
      {
        EvalT val = mat[i][j];
        for (int k = 0; k < i; ++k)
          val -= L[i][k] * U[k][j];

        if (i == j && fit(val, 0)) return 0;
        U[i][j] = val;
      }
      else
      {
        EvalT val = mat[i][j];
        for (int k = 0; k < j; ++k)
          val -= L[i][k] * U[k][j];

        L[i][j] = val / U[j][j];
      }
    }
  }

  EvalT determinant = 1;

  for (int i = 0; i < n; ++i)
  {
    determinant *= U[i][i];
  }

  return determinant;
}

template<typename T, typename EvalT=double>
EvalT qr_det(const Matrix<T>& mat)
{
  if(!mat.is_sqare())
    throw std::invalid_argument("Input Matrix object is not square");

  size_t n = mat.dims().n;

  Matrix<EvalT> a = eval_matrix(mat);
  Matrix<EvalT> q {n, n}; 
  Matrix<EvalT> r {n, n};

  for (int k = 0; k < n; ++k)
  {
    EvalT s = 0;
    for (int j = 0; j < n; ++j) s += a[j][k] * a[j][k];
    r[k][k] = std::sqrt(s);

    for (int j = 0; j < n; ++j) q[j][k] = a[j][k] / r[k][k];

    for (int i = k + 1; i < n; ++i)
    {
      EvalT s = 0;
      for (int j = 0; j < n; ++j) s += a[j][i] * q[j][k];
      r[k][i] = s;

      for (int j = 0; j < n; ++j) a[j][i] = a[j][i] - r[k][i] * q[j][k];
    }
  }
  EvalT determinant = 1 - 2*std::signbit(ge_det(q));

  for (int i = 0; i < n; ++i)
  {
    determinant *= r[i][i];
  }

  return determinant;
}

template<typename T, typename EvalT=double>
EvalT det(const Matrix<T>& mat)
{
  return ge_det(mat);
}

}