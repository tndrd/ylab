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

// a <= t <= b
inline bool interval_fit(double t, double a, double b) noexcept
{
  bool cond1 = (t >= a) && (t <= b);
  bool cond2 = fit(t, a);
  bool cond3 = fit(t, b);
  return cond1 || cond2 || cond3;
}

inline bool intervals_intersect(double min1, double max1, double min2, double max2) noexcept
{
  if(fit(max1,min2) || fit(max2, min1))
  {
    return true;
  }

  if (max1 < min2 || max2 < min1)
  {
    return false;
  }

  return true;
}
}