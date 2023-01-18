#pragma once

#include "Vec3D.hpp"
#include "Line3D.hpp"
#include "Triangle3D.hpp"
#include "triangle_intersections.hpp"
#include "intervals.hpp"

namespace HW3D
{

inline bool base_intersect(const Point3D& pt1, const Point3D& pt2) noexcept
{
  return pt1 == pt2;
}

inline bool base_intersect(const Point3D& pt, const LineSeg3D& l) noexcept
{
  Vec3D p = l.get_p();
  Vec3D a = l.get_a();

  Vec3D norm = p - project_v(p, a);

  Vec3D direction = (pt - norm);

  if (!abseq(direction.normalize(), a.normalize()))
    return false;

  double t = direction.length() / a.length();

  return l.check_param(t);
}

inline bool base_intersect(const Point3D& pt, const Triangle3D& tr) noexcept
{
  Plane3D plane = tr.get_plane();

  if ((pt * plane.get_n()) != plane.get_s())
    return false;

  auto normals = get_edge_normals(tr);
  for (int i = 0; i < 3; ++i)
  {
    auto interval = ComputeInterval(tr, normals[i]);
    
    double min = interval[0];
    double max = interval[1];
    
    double t = pt * normals[i];
    if (!interval_fit(t, min, max)) return false;
  }
  return true;
}

inline bool base_intersect(const LineSeg3D& l1, const LineSeg3D& l2) noexcept
{
  LineRelation rel = get_line_relation(l1, l2);
  LineRelation::State state = rel.get_state();
  return state == LineRelation::INTERSECTING || state == LineRelation::COINCIDENT;
}

inline bool base_intersect(const LineSeg3D& l, const Triangle3D& tr) noexcept
{
  std::vector<double> intersections = intersect_with(tr, l);
  return intersections.size() > 0;
}

inline bool base_intersect(const Triangle3D& tr1, const Triangle3D& tr2) noexcept
{
  return intersect_triangles(tr1, tr2);
}

inline bool intersect(const Triangle3D& tr1, const Triangle3D& tr2)
{
  std::vector<Point3D> unique1 = tr1.unique();
  std::vector<Point3D> unique2 = tr2.unique();

  switch (unique1.size())
  {
    case 1: switch(unique2.size())
            {
              case 1:  return base_intersect(Point3D{unique1[0]}, Point3D{unique2[0]});
              case 2:  return base_intersect(Point3D{unique1[0]}, LineSeg3D{unique2[0], unique2[1]});
              case 3:  return base_intersect(Point3D{unique1[0]}, Triangle3D{unique2[0], unique2[1], unique2[2]});
              default: throw std::runtime_error("Incorrect unique size");
            } 
    case 2: switch(unique2.size())
            {
              case 1:  return base_intersect(Point3D{unique2[0]},  LineSeg3D{unique1[0], unique1[1]});
              case 2:  return base_intersect(LineSeg3D{unique1[0], unique1[1]}, LineSeg3D{unique2[0], unique2[1]});
              case 3:  return base_intersect(LineSeg3D{unique1[0], unique1[1]}, Triangle3D{unique2[0], unique2[1], unique2[2]});
              default: throw std::runtime_error("Incorrect unique size");
            }
    case 3: switch(unique2.size())
            {
              case 1:  return base_intersect(Point3D{unique2[0]}, Triangle3D{unique1[0], unique1[1], unique1[2]});
              case 2:  return base_intersect(LineSeg3D{unique2[0], unique2[1]}, Triangle3D{unique1[0], unique1[1], unique1[2]});
              case 3:  return base_intersect(Triangle3D{unique1[0], unique1[1], unique1[2]}, Triangle3D{unique2[0], unique2[1], unique2[2]});
              default: throw std::runtime_error("Incorrect unique size");
            }
    default:  throw std::runtime_error("Incorrect unique size");
  }
}

}