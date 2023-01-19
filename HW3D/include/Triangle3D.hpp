#pragma once

#include "Plane3D.hpp"
#include "LineSeg3D.hpp"
#include "primitive_intersections.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

namespace HW3D
{
class Triangle3D final
{
  // 3D Triangle class
  // Stores and accesses triangle information
  // Conists of three vertices
  std::array<Point3D, 3> vertices_;

  public:
  Triangle3D(const Point3D& p1, const Point3D& p2, const Point3D& p3):
  vertices_({p1, p2, p3})
  {
    //get_plane(); // Will throw an exception if any two points coincide
  }

  // Returns plane of triangle
  Plane3D get_plane() const
  {
    return {vertices_[0], vertices_[1], vertices_[2]};
  }

  // Returns specified edge of triangle
  LineSeg3D get_edge(u_char i1, u_char i2) const
  {
    return {vertices_[i1], vertices_[i2]}; // Will throw an exception if points coincide
  }

  // Returns specified vertice of triangle
  Vec3D get_vertice(u_char i) const
  {
    return vertices_[i]; // Will throw an exception if index out of range
  }
  
  // Returns vector of unique vertices
  std::vector<Point3D> unique() const noexcept
  {
    std::vector<Point3D> unique_points;
    for (int i = 0; i < 3; ++i)
    {
      bool is_unique = true;
      for (int k = 0; k < unique_points.size(); ++k)
        if (get_vertice(i) == unique_points[k])
        {
          is_unique = false;
          break;
        }
      if (is_unique)
        unique_points.push_back(get_vertice(i));
    }
    return unique_points;
  }
  
  // Creates vector of unique vertices and deletes ones which lay
  // on line between two other vertices
  std::vector<Point3D> simplify() const noexcept
  {
    std::vector<Point3D> unique_points = unique();
    if (unique_points.size() < 3) return unique_points;
    
    for (int i = 0; i < 3; ++i)
    {
      Vec3D edge      = get_vertice((i + 1) % 3) - get_vertice(i);
      Vec3D direction = get_vertice((i + 2) % 3) - get_vertice(i);

      assert(edge.length() != 0);
      assert(direction.length() != 0);

      if (edge.normalize() == direction.normalize())
      {
        double t = direction.length() / edge.length();
        if (interval_fit(t, 0, 1))
        {
          return {unique_points[i], unique_points[(i + 1) % 3]};
        }
      }
    }
    return unique_points;
  }
};

// Returns edge normal directions
inline std::vector<Vec3D> get_edge_normals(const Triangle3D& tr) noexcept
{
  std::vector<Vec3D> normal_lines;
  
  Plane3D plane = tr.get_plane();

  for (int i = 0; i < 3; i++)
  {
    Point3D v1 = tr.get_vertice(i);
    Point3D v2 = tr.get_vertice((i + 1) % 3);
    LineSeg3D edge = {v1, v2};

    Vec3D direction = vecmul(plane.get_n(), edge.get_a()).normalize(); 
    normal_lines.push_back(direction);
  }

  return normal_lines;
}

// Intersects all the triangle edges with a specific line
// Returns vector of t params, which represent intersection points through line equation
// This vector is sorted in ascending order
// Number of points may be zero, two, three or infinity
// If only one vertice lay on desired line, we consider the intersection count to be equal two
// That's because in this case two edges intersect the line in one point
template<typename LineT>
inline std::vector<double> intersect_with(const Triangle3D& tr, const LineT& line) noexcept
{
  std::vector<double> intersections;
  
  for (int i = 0; i < 3; i++)
  {
    Point3D v1 = tr.get_vertice(i);
    Point3D v2 = tr.get_vertice((i + 1) % 3);
    LineSeg3D edge = {v1, v2};

    LineRelation rel = get_line_relation(edge, line);
    using state_t = LineRelation::State;
  
    switch(rel.get_state())
    {
      case state_t::COINCIDENT:
      {
        break;
      }

      case state_t::PARALLEL:
      {
        break;
      }

      case state_t::NON_INTERSECTING:
      {
        break;
      }

      case state_t::INTERSECTING:
      {
        intersections.push_back(rel.get_t2());
        break;
      }
    }
  }

  std::sort(intersections.begin(), intersections.end());
  return intersections;
}

}