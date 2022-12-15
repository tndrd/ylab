#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <exception>
#include "intervals.hpp"

namespace HW3D
{

// Just a regular 3D vector
// With overloaded vector operations
struct Vec3D
{
  double x = NAN;
  double y = NAN;
  double z = NAN;

  bool valid() const
  {
    return !(std::isnan(x) || std::isnan(y) || std::isnan(z));
  }

  Vec3D(double newx, double newy, double newz): x(newx), y(newy), z(newz)
  {
    if (!valid()) 
      throw std::invalid_argument("Vec3D construction with NAN"); // just for safety purposes
  }

  Vec3D(const Vec3D& vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;
  }

  double length() const noexcept
  {
    return std::sqrt(x*x + y*y + z*z);
  }

  Vec3D normalize() const // Potential zero division exception
  {
    Vec3D tmp {*this};
    return tmp /= tmp.length();
  }

  Vec3D& operator+= (const Vec3D& rhs) noexcept
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Vec3D operator+ (const Vec3D& rhs) const noexcept
  {
    Vec3D tmp {*this};
    return tmp += rhs;
  }

  Vec3D& operator-= (const Vec3D& rhs) noexcept
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  Vec3D operator- (const Vec3D& rhs) const noexcept
  {
    Vec3D tmp {*this};
    return tmp -= rhs;
  }

  Vec3D& operator*= (double k) noexcept
  {
    x *= k;
    y *= k;
    z *= k;
    return *this;
  }

  Vec3D operator* (double k) const noexcept
  {
    Vec3D tmp {*this};
    return tmp *= k;
  }

  Vec3D& operator/= (double k) // Potential zero division exception 
  {
    x /= k;
    y /= k;
    z /= k;
    return *this;
  }

  Vec3D operator/ (double k) const // Potential zero division exception
  {
    Vec3D tmp {*this};
    return tmp /= k;
  }

  bool operator== (const Vec3D& rhs) const noexcept
  {
    return fit(x, rhs.x) && fit(y, rhs.y) && fit(z, rhs.z);
  }

  Vec3D operator- () const noexcept
  {
    return {-x, -y, -z};
  }

  double operator* (const Vec3D& rhs) const noexcept
  {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  }
};

inline Vec3D operator* (double k, const Vec3D& rhs) noexcept
{
  return rhs * k;
}

inline Vec3D project_v(const Vec3D& which_vec, const Vec3D& where_vec) // Potential zero division exception
{
  return ((which_vec * where_vec) / (where_vec * where_vec)) * where_vec; 
}

inline bool abseq(const Vec3D& v1, const Vec3D& v2) noexcept
{
  return v1 == v2 || v1 == -v2;
}

inline double det(const Vec3D& v1, const Vec3D& v2, const Vec3D& v3) noexcept
{
  double min1 = v1.x * (v2.y*v3.z - v2.z*v3.y);
  double min2 = v1.y * (v2.x*v3.z - v2.z*v3.x);
  double min3 = v1.z * (v2.x*v3.y - v2.y*v3.x);

  return min1 - min2 + min3;
}

inline Vec3D vecmul(const Vec3D& v1, const Vec3D& v2) noexcept
{
  double min1 = v1.y*v2.z - v1.z*v2.y;
  double min2 = v1.x*v2.z - v1.z*v2.x;
  double min3 = v1.x*v2.y - v1.y*v2.x;

  return {min1, -min2, min3};
}

inline std::ostream& operator<<(std::ostream& os, const Vec3D& v)
{
    os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
    return os;
}

using Point3D = Vec3D; // Just for aestethical purposes. Just to emphasise that Point3D is a radius-vector

};