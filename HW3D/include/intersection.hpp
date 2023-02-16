#pragma once

#include "Vec3D.hpp"
#include "Line3D.hpp"
#include "Triangle3D.hpp"
#include "triangle_intersections.hpp"
#include "intervals.hpp"

namespace HW3D
{

inline bool base_intersect(const Point3D& pt1, const Point3D& pt2) 
{
  //std::cout << "PP" << std::endl;
  return pt1 == pt2;
}

inline bool base_intersect(const Point3D& pt, const LineSeg3D& l) 
{
  //std::cout << "PL" << std::endl;
  Vec3D p = l.get_p();
  Vec3D a = l.get_a();
  //std::cout << pt << std::endl;
  //std::cout << l << std::endl;
  if (pt == p) 
  {
    //std::cout << "1" << std::endl;
    return true;
  }
  Vec3D direction = (pt - p);

  if (!(direction.normalize() == a.normalize()))
  {
    //std::cout << "2" << std::endl;
    return false;
  }
  double t2 = (direction * direction) / (a * a);
  bool param = l.check_param(std::sqrt(t2));
  //std::cout << "param: " << t2 << std::endl; 
  return l.check_param(std::sqrt(t2));
}

inline bool base_intersect(const Point3D& pt, const Triangle3D& tr) 
{
  //std::cout << "PT" << std::endl;
  Plane3D plane = tr.get_plane();

  double s0 = pt * plane.get_n();
  double s  = plane.get_s();
  if (!fit(s0, s)) return false;
  
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

inline bool base_intersect(const LineSeg3D& l1, const LineSeg3D& l2) 
{
  //std::cout << "LL" << std::endl;
  LineRelation rel = get_line_relation(l1, l2);
  LineRelation::State state = rel.get_state();

  switch(state)
  {
    case LineRelation::NON_INTERSECTING: return false;
    
    case LineRelation::COINCIDENT:
    case LineRelation::INTERSECTING: return true;
    
    case LineRelation::PARALLEL:
    {
      Point3D p1 = l1.get_p();
      Point3D p2 = p1 + l1.get_a();
      return base_intersect(p1, l2) || base_intersect(p2, l2);   
    }
  }

  return state == LineRelation::INTERSECTING || state == LineRelation::COINCIDENT;
}

inline bool base_intersect(const LineSeg3D& l, const Triangle3D& tr) 
{
  //std::cout << "LT" << std::endl;
  Plane3D plane = tr.get_plane();
  LinePlaneRelation rel = get_line_plane_relation(l, plane);

  if (rel.get_state() == LinePlaneRelation::NON_INTERSECTING || 
      rel.get_state() == LinePlaneRelation::PARALLEL) return false;

  if (rel.get_state() == LinePlaneRelation::COINCIDENT)
  {
    std::vector<double> intersections = intersect_with(tr, l);
    return intersections.size() > 0;
  }

  if (rel.get_state() == LinePlaneRelation::INTERSECTING)
  {
    Point3D pt = get_line_plane_intersection(l, plane, rel);
    return base_intersect(pt, tr);
  }
  return false;
}

inline bool base_intersect(const Triangle3D& tr1, const Triangle3D& tr2) 
{
  //std::cout << "TT" << std::endl;
  return intersect_triangles(tr1, tr2);
}


// Recieves two vectors of unique points and applies intersection specific algorithm
// corresponding to each vector's size
//
// I am not sure this is a best way to choose intersection algorithm
// I think other implementations may use dynamical polymorhism but i
// don't want to use it due to potential overhead
inline bool intersect(const std::vector<Point3D>& unique1, const std::vector<Point3D>& unique2)
{
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

inline bool intersect(const Triangle3D& tr1, const Triangle3D& tr2)
{
  std::vector<Point3D> unique1 = tr1.simplify();
  std::vector<Point3D> unique2 = tr2.simplify();
  return intersect(unique1, unique2);
}

}