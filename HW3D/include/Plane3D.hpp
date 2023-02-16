#pragma once

#include "Vec3D.hpp"
#include "LineInf3D.hpp"

namespace HW3D
{

class Plane3D final
{

  // Plane equation: (n, P) = s
  // n - plane normal, |n| = 1
  // s - plane offset
  // Equation should be tranformed into one with s >= 0 

  Vec3D  n_;
  double s_;

  Vec3D calc_n(const Point3D& a, const Point3D& b, const Point3D& c) const noexcept
  {
    return vecmul(a - b, c - b);
  }

  double calc_s(const Point3D& a, const Point3D& b, const Point3D& c) const noexcept
  {
    return det(c, b, a);
  }

  // Normalization is an inportant part of defining planes in 3D
  // After the normalization parameters and their planes become one-to-one correspondent 
  void normalize() noexcept
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
    if (n_ == Vec3D{0, 0, 0}) throw std::invalid_argument("Plane normal vector can not be zero");
    normalize();
  }

  Plane3D(const Point3D& a, const Point3D& b, const Point3D& c): Plane3D(calc_n(a, b, c), calc_s(a, b, c)) {}

  Vec3D  get_n() const noexcept { return n_; }
  double get_s() const noexcept { return s_; }

  // Because of normalization planes wont be equal if their s-params are not same
  bool is_coincident(const Plane3D& p) const noexcept
  { 
    if (fit(p.s_, s_))
    {
      if (!fit(s_, 0)) return (p.n_ == n_);
      else             return abseq(p.n_, n_);
    }
    return false;
  }

  bool is_parallel(const Plane3D& p) const noexcept
  {
    return abseq(n_, p.n_);
  }

  LineInf3D make_line(const Vec3D& direction) const noexcept
  {
    return {direction, n_ * s_};
  }
};

inline std::ostream& operator<<(std::ostream& os, const Plane3D& p)
{
    os << "n: " << p.get_n() << " s:" << p.get_s();
    return os;
}

inline double dist(const Plane3D& plane, const Point3D& point)
{
  Point3D S = plane.get_s() * plane.get_n();
  double d = plane.get_n() * (point - S);
  return d;
}

}