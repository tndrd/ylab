#pragma once

#include "Line3D.hpp"

namespace HW3D
{
  // Line segment class
  // Can be constructed with start and end points (p1,p2)
  // We define the line equation [v = p + at] as v = p1 + (p2 - p1)t
  // This way only points that are defined with 0<t<1 belong to this segment
  class LineSeg3D final: public Line3D
  {
    public:
    LineSeg3D(const Point3D& p1, const Point3D& p2): Line3D(p2 - p1, p1) {}

    bool is_coincident(const LineSeg3D& ls) const noexcept
    {
      Point3D p11 = p_;
      Point3D p12 = p_ + a_;

      Point3D p21 = ls.p_;
      Point3D p22 = ls.p_ + ls.a_;

      bool cond1 = (p11 == p21) && (p12 == p22);
      bool cond2 = (p12 == p21) && (p11 == p22);

      return cond1 || cond2; 
    }

    bool is_coincident(const LineInf3D& li) const noexcept
    { 
      return li.is_coincident(*this);
    }

    static bool check_param(double t) noexcept
    {
      return interval_fit(t, 0, 1); 
    }
  };
}