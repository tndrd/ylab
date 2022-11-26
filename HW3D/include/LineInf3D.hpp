#pragma once

#include "Line3D.hpp"

namespace HW3D
{
  // Infinite line class
  // Can be constructed with a and p given explicitly,
  // There are no restrictions on parameter t
  class LineInf3D: public Line3D
  {

    private:

    void normalize()
    {
      a_ = a_.normalize();
      p_ = p_ - project_v(p_, a_);
    }

    public:
    LineInf3D(const Vec3D& a, const Point3D& p={0,0,0}): Line3D(a, p)
    {
      normalize();
    }

    bool is_coincident(const Line3D& li) const
    { 
      if (!is_parallel(li)) return false;

      Vec3D p1 = p_ - project_v(p_, a_);
      Vec3D p2 = li.get_p() - project_v(li.get_p(), a_);

      return p1 == p2; 
    }

    bool is_coincident(const LineInf3D& li) const
    { 
      return abseq(a_, li.get_a()) && p_ == li.get_p(); 
    }

    bool check_param(double t) const { return true; }
  }; 
}