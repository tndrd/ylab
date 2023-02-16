#pragma once

#include <array>
#include <cmath>
#include <iostream>

namespace HW3D
{

inline const double FIT_TOLERANCE = 0.0001;

inline bool fit(double a, double b) noexcept
{
  return std::abs(a - b) < FIT_TOLERANCE;
}

inline bool greater(double a, double b)
{
  return a > b + FIT_TOLERANCE;
}

inline bool lesser(double a, double b)
{
  return a < b - FIT_TOLERANCE;
}

inline bool egreater(double a, double b)
{
  return a >= b - FIT_TOLERANCE;
}

inline bool elesser(double a, double b)
{
  return a <= b + FIT_TOLERANCE;
}

// a <= t <= b
inline bool interval_fit(double t, double a, double b) noexcept
{
  return egreater(t, a) && elesser(t, b);
}

inline bool intervals_intersect(double min1, double max1, double min2, double max2) noexcept
{
  return egreater(max1, min2) && elesser(min1, max2);
}
}