#pragma once

#include "Vec3D.hpp"
#include "LineInf3D.hpp"

namespace HW3D
{

class Plane3D
{

  // Plane equation: (n, P) = s
  // n - plane normal, |n| = 1
  // s - plane offset
  // Equation should be tranformed into one with s >= 0 

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

  // Normalization is an inportant part of defining planes in 3D
  // After the normalization parameters and their planes become one-to-one correspondent 
  void normalize()
  {
    double nlen = n_.length();

    double ssgn = 1;
    if (s_ != 0)
    {
      ssgn = s_ / std::abs(s_);
    }

    n_ = n_ / nlen * ssgn;
    s_ = s_ / nlen * ssgn; 
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

  // Because of normalization planes wont be equal if their s-params are not same
  bool is_coincident(const Plane3D& p) const
  { 
    if (fit(p.s_, s_))
    {
      if (!fit(s_, 0)) return (p.n_ == n_);
      else             return abseq(p.n_, n_);
    }
    return false;
  }

  bool is_parallel(const Plane3D& p) const
  {
    return abseq(n_, p.n_);
  }

  LineInf3D make_line(const Vec3D& direction)
  {
    return {direction, n_ * s_};
  }

};

}