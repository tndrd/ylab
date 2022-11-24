#include <array>
#include <cmath>

namespace HW3D
{

const double FIT_TOLERANCE = 0.00001;

bool fit(double a, double b)
{
  return (a - b) < FIT_TOLERANCE;
}

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
    if (!valid()) throw; // just for safety purposes
  }

  Vec3D(const Vec3D& vec)
  {
    if (!vec.valid()) throw;
    
    x = vec.x;
    y = vec.y;
    z = vec.z;
  }

  double length() const
  {
    return std::sqrt(x*x + y*y + z*z);
  }

  void normalize()
  {
    double absval = length();

    x /= absval;
    y /= absval;
    z /= absval;
  }
};

Vec3D operator+(const Vec3D& v1, const Vec3D& v2)
{
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3D operator-(const Vec3D& v1, const Vec3D& v2)
{
  return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

double operator*(const Vec3D& v1, const Vec3D& v2)
{
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vec3D operator*(const Vec3D& v, const double k)
{
  return {v.x * k, v.y * k, v.z * k};
}

Vec3D operator*(const double k, const Vec3D& v)
{
  return {v.x * k, v.y * k, v.z * k};
}

Vec3D operator/(const Vec3D& v, const double k)
{
  return {v.x / k, v.y / k, v.z / k};
}

Vec3D operator/(const double k, const Vec3D& v)
{
  return {v.x / k, v.y / k, v.z / k};
}

Vec3D operator-(const Vec3D& v)
{
  return {-v.x, -v.y, -v.z};
}

Vec3D project_v(const Vec3D& which_vec, const Vec3D& where_vec)
{
  return (which_vec * where_vec / where_vec.length()) * where_vec; 
}

Vec3D abs(const Vec3D& v)
{
  return {std::abs(v.x), std::abs(v.y), std::abs(v.z)};
}

bool operator==(const Vec3D& v1, const Vec3D& v2)
{
  return fit(v1.x, v2.x) && fit(v1.y, v2.y) && fit(v1.z, v2.z);
}

using Point3D = Vec3D; // Just for aestethical purposes. Just to emphasise that Point3D is a radius-vector

};