/* Bounding box class */

#pragma once
#include <array>

#include "Vec3D.hpp"
#include "Triangle3D.hpp"

namespace HW3D
{

struct BoundingBox final
{
  data_t x1, x2, y1, y2, z1, z2;

  BoundingBox(const Triangle3D& tr)
  {
    Point3D v0 = tr.get_vertice(0);
    
    x1 = x2 = v0.x;
    y1 = y2 = v0.y;
    z1 = z2 = v0.z;

    for(int i = 1; i < 3; ++i)
    {
      Point3D v = tr.get_vertice(i);

      x1 = std::min(x1, v.x);
      x2 = std::max(x2, v.x);

      y1 = std::min(y1, v.y);
      y2 = std::max(y2, v.y);

      z1 = std::min(z1, v.z);
      z2 = std::max(z2, v.z);
    }
  }
};

inline bool axis_intersect(data_t min1, data_t max1, data_t min2, data_t max2)
{
  return max1 >= min2 && min1 <= max2;
}

inline bool intersects(const BoundingBox& bb1, const BoundingBox& bb2)
{
  return axis_intersect(bb1.x1, bb1.x2, bb2.x1, bb2.x2) && 
         axis_intersect(bb1.y1, bb1.y2, bb2.y1, bb2.y2) &&
         axis_intersect(bb1.z1, bb1.z2, bb2.z1, bb2.z2);
}

}