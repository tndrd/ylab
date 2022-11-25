#pragma once

#include "Line3D.hpp"

namespace HW3D
{
  // Ray class
  // Can be constructed with start point p and direction a
  // Parameter t should be greater than zero
  class LineRay3D: public Line3D
  {
    public:
    LineRay3D(const Vec3D& a, const Point3D& p): Line3D(a, p) {}

    bool is_coincident(const LineRay3D& lr) const
    {
      return a_ == lr.a_ && p_ == lr.p_; 
    }

    bool check_param(double t) const
    {
      return t >= 0; 
    }
  };
}