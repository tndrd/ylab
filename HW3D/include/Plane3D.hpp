#pragma once

#include "Vec3D.hpp"
#include "lines.hpp"

namespace HW3D
{

class Plane3D final
{

  // Plane equation: (n, P) = s
  // n - plane normal, |n| = 1
  // s - plane offset
  // Equation should be tranformed into one with s >= 0 

  Vec3D  n_;
  data_t s_;

  Vec3D calc_n(const Point3D& a, const Point3D& b, const Point3D& c) const noexcept
  {
    return vecmul(a - b, c - b);
  }

  data_t calc_s(const Point3D& a, const Point3D& b, const Point3D& c) const noexcept
  {
    return det(c, b, a);
  }

  // Normalization is an inportant part of defining planes in 3D
  // After the normalization parameters and their planes become one-to-one correspondent 
  void normalize() noexcept
  {
    data_t nlen = n_.length();

    data_t ssgn = 1;
    if (s_ != 0)
    {
      ssgn = s_ / std::abs(s_);
    }

    n_ = n_ / nlen * ssgn;
    s_ = s_ / nlen * ssgn; 
  }

  public:
  Plane3D(const Vec3D& n = {0,0,0}, data_t s=NAN): n_(n), s_(s)
  {
    assert(!is_zero(n));
    normalize();
  }

  Plane3D(const Point3D& a, const Point3D& b, const Point3D& c): Plane3D(calc_n(a, b, c), calc_s(a, b, c)) {}

  Vec3D  get_n() const noexcept { return n_; }
  data_t get_s() const noexcept { return s_; }

  bool is_parallel(const Plane3D& p2) const
  {
    return collinear(get_n(), p2.get_n());
  }

  bool is_coincident(const Plane3D& p2) const
  {
    data_t s1 = get_s();
    data_t s2 = p2.get_s();

    Vec3D  n1 = get_n();
    Vec3D  n2 = p2.get_n();

    if (fit(s1, s2))
    {
      if (!fit(s1, 0)) return (n1 == n2);
      else             return collinear(n1, n2);
    }
    return false;
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

inline data_t dist(const Plane3D& plane, const Point3D& point)
{
  data_t d = (plane.get_n() * point) - plane.get_s();
  return d;
}

// Because of normalization planes wont be equal if their s-params are not same
inline bool coincident(const Plane3D& p1, const Plane3D& p2) noexcept
{ 

  data_t s1 = p1.get_s();
  data_t s2 = p2.get_s();

  Vec3D  n1 = p1.get_n();
  Vec3D  n2 = p2.get_n();

  if (fit(s1, s2))
  {
    if (!fit(s1, 0)) return (n1 == n2);
    else             return collinear(n1, n2);
  }
  return false;
}

inline bool parallel(const Plane3D& p1, const Plane3D& p2) noexcept
{
  return collinear(p1.get_n(), p2.get_n());
}

}