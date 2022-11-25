#pragma once

#include "Line3D.hpp"

namespace HW3D
{
  // Infinite line class
  // Can be constructed with a and p given explicitly,
  // There are no restrictions on parameter t
  class LineInf3D: public Line3D
  {
    public:
    LineInf3D(const Vec3D& a, const Point3D& p = {0,0,0}): Line3D(a, p) {}

    bool is_coincident(const LineInf3D& li)
    { 
      Vec3D p1 = p_ - project_v(p_, a_);
      Vec3D p2 = li.p_ - project_v(li.p_, a_);

      return is_parallel(li) && (p1 == p2); 
    }

    bool check_param(double t) const { return true; }
  }; 
}