#pragma once

#include "Vec3D.hpp"
//#include "LineInf3D.hpp"

namespace HW3D
{

class Plane3D
{
  Vec3D  n_;
  double s_;

  Vec3D calc_n(const Point3D& a, const Point3D& b, const Point3D& c) const
  {
    return vecmul(a - b, c - b);
  }

  double calc_s(const Point3D& a, const Point3D& b, const Point3D& c) const
  {
    return det(c, b, a);
  }

  void normalize()
  {
    double nlen = n_.length();
    n_ = n_ / nlen;
    s_ = s_ / nlen; 
  }

  public:
  Plane3D(const Vec3D& n, double s): n_(n), s_(s)
  {
    normalize();
  }

  Plane3D(const Point3D& a, const Point3D& b, const Point3D& c):
    n_(calc_n(a, b, c)),
    s_(calc_s(a, b, c))
  {
    normalize();
  }

  Vec3D get_n() const
  {
    return n_;
  }

  double get_s() const
  {
    return s_;
  }

  bool is_coincident(const Plane3D& p) const
  {
    bool cond1 = (p.n_ == n_) && (p.s_ == s_);
    bool cond2 = (p.n_ == - n_) && (p.s_ == - s_);

    return cond1 || cond2;
  }

  bool is_parallel(const Plane3D& p) const
  {
    return abs(n_) == abs(p.n_);
  }

  LineInf3D make_line(const Vec3D& direction)
  {
    return {direction, n_ * s_};
  }

};

}