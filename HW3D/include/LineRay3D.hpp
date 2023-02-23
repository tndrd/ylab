#pragma once

#include "Line3D.hpp"

namespace HW3D
{
  // Ray class
  // Can be constructed with start point p and direction a
  // Parameter t should be greater than zero
  // Disclaimer: This class is not used
  class LineRay3D final: public Line3D
  {
    public:
    LineRay3D(const Vec3D& a, const Point3D& p): Line3D(a, p)
    {
      a_ = a_.normalize();
    }

    bool is_coincident(const LineRay3D& lr) const noexcept
    {
      return a_ == lr.a_ && p_ == lr.p_; 
    }

    static bool check_param(data_t t) noexcept
    {
      return t >= 0; 
    }
  };
}