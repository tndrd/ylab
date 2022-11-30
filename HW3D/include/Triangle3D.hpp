#include "Plane3D.hpp"
#include "LineSeg3D.hpp"
#include "intersections.hpp"
#include <array>
#include <vector>
#include <algorithm>

namespace HW3D
{
class Triangle3D
{
  // Triangle in 3D class
  // Conists of three vertices
  std::array<Point3D, 3> vertices_;

  public:
  Triangle3D(const Point3D& p1, const Point3D& p2, const Point3D& p3):
  vertices_({p1, p2, p3}) {}

  // Returns plane of triangle
  Plane3D get_plane() const
  {
    return {vertices_[0], vertices_[1], vertices_[2]};
  }

  // Returns specified edge of triangle
  LineSeg3D get_edge(u_char i1, u_char i2) const
  {
    return {vertices_[i1], vertices_[i2]};
  }

  // Returns specified vertice of triangle
  Vec3D get_vertice(u_char i) const
  {
    return vertices_[i];
  }

  // Returns edge normal directions
  std::vector<Vec3D> get_edge_normals() const
  {
    std::vector<Vec3D> normal_lines;
    
    Plane3D plane = get_plane();

    for (int i = 0; i < 3; i++)
    {
      Point3D v1 = vertices_[i];
      Point3D v2 = vertices_[(i+1)%3];
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
  std::vector<double> intersect_with(const LineT& line) const
  {
    std::vector<double> intersections;
    
    for (int i = 0; i < 3; i++)
    {
      Point3D v1 = vertices_[i];
      Point3D v2 = vertices_[(i+1)%3];
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

};

}