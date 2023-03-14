/* Bounding box class */

#pragma once
#include <array>

#include "Vec3D.hpp"
#include "Triangle3D.hpp"

namespace HW3D
{

// Axis-aligned bounding box class (AABB)
class BoundingBox final
{
  data_t x1, x2, y1, y2, z1, z2;

  public:
  BoundingBox(std::vector<Point3D> points)
  {
    assert(points.size() > 0);

    Point3D v0 = points[0];
    
    x1 = x2 = v0.x;
    y1 = y2 = v0.y;
    z1 = z2 = v0.z;

    for(size_t i = 1; i < points.size(); ++i)
    {
      Point3D v = points[i];

      x1 = std::min(x1, v.x);
      x2 = std::max(x2, v.x);

      y1 = std::min(y1, v.y);
      y2 = std::max(y2, v.y);

      z1 = std::min(z1, v.z);
      z2 = std::max(z2, v.z);
    }
  }

  bool intersects(const BoundingBox& rhs) const
  {
    return intervals_intersect(x1, x2, rhs.x1, rhs.x2) && 
           intervals_intersect(y1, y2, rhs.y1, rhs.y2) &&
           intervals_intersect(z1, z2, rhs.z1, rhs.z2);
  }
};

inline bool intersects(const BoundingBox& bb1, const BoundingBox& bb2)
{
  return bb1.intersects(bb2);
}


}