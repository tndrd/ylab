#pragma once

#include <array>
#include <cmath>
#include <iostream>

namespace HW3D
{

using data_t = double;

static const data_t FIT_TOLERANCE = 0.0001;

inline bool fit(data_t a, data_t b) noexcept
{
  return std::abs(a - b) < FIT_TOLERANCE;
}

inline bool greater(data_t a, data_t b)
{
  return a > b + FIT_TOLERANCE;
}

inline bool lesser(data_t a, data_t b)
{
  return a < b - FIT_TOLERANCE;
}

inline bool egreater(data_t a, data_t b)
{
  return a >= b - FIT_TOLERANCE;
}

inline bool elesser(data_t a, data_t b)
{
  return a <= b + FIT_TOLERANCE;
}

// a <= t <= b
inline bool interval_fit(data_t t, data_t a, data_t b) noexcept
{
  return egreater(t, a) && elesser(t, b);
}

inline bool intervals_intersect(data_t min1, data_t max1, data_t min2, data_t max2) noexcept
{
  return egreater(max1, min2) && elesser(min1, max2);
}
}