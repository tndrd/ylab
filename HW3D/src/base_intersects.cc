/* Intersection check algorithms implementation */

#include "base_intersects.hpp"

namespace HW3D
{

bool base_intersects(const Point3D& pt1, const Point3D& pt2)
{
  return pt1 == pt2;
}

bool base_intersects(const Point3D& pt, const LineSeg3D& l)
{
  Vec3D p = l.get_p();
  Vec3D a = l.get_a();
  if (pt == p) 
  {
    return true;
  }

  Vec3D direction = (pt - p);

  if (!(direction.normalize() == a.normalize()))
  {
    return false;
  }
  data_t t2 = (direction * direction) / (a * a);
  return l.check_param(std::sqrt(t2));
}

bool base_intersects(const Point3D& pt, const PlaneTriangle& plt)
{
  const Triangle3D& tr = plt.triangle;
  const Plane3D& plane = plt.plane;

  data_t s0 = pt * plane.get_n();
  data_t s  = plane.get_s();
  if (!fit(s0, s)) return false;
  
  auto normals = get_edge_normals(tr);
  for (int i = 0; i < 3; ++i)
  {
    auto interval = ComputeInterval(tr, normals[i]);
    
    data_t min = interval[0];
    data_t max = interval[1];
    
    data_t t = pt * normals[i];
    if (!interval_fit(t, min, max)) return false;
  }
  return true;
}

bool base_intersects(const LineSeg3D& l, const Point3D& pt)
{
  return base_intersects(pt, l); 
}

bool base_intersects(const LineSeg3D& l1, const LineSeg3D& l2)
{
  LineRelation rel = get_line_relation(l1, l2);
  LineRelation::State state = rel.get_state();

  switch(state)
  {
    case LineRelation::NON_INTERSECTING: return false;
    
    case LineRelation::INTERSECTING: return true;
    
    case LineRelation::PARALLEL:
    {
      Point3D p1 = l1.get_p();
      Point3D p2 = p1 + l1.get_a();
      return base_intersects(p1, l2) || base_intersects(p2, l2);   
    }
    default: return false;
  }
}

bool base_intersects(const LineSeg3D& l, const PlaneTriangle& plt)
{
  const Triangle3D& tr = plt.triangle;
  const Plane3D& plane = plt.plane;

  LinePlaneRelation rel = get_line_plane_relation(l, plane);

  if (rel.get_state() == LinePlaneRelation::NON_INTERSECTING || 
      rel.get_state() == LinePlaneRelation::PARALLEL) return false;

  if (rel.get_state() == LinePlaneRelation::COINCIDENT)
  {
    std::vector<data_t> intersections = intersect_with(tr, l);
    return intersections.size() > 0;
  }

  if (rel.get_state() == LinePlaneRelation::INTERSECTING)
  {
    Point3D pt = get_line_plane_intersection(l, plane, rel);
    return base_intersects(pt, plt);
  }
  return false;
}

bool base_intersects(const PlaneTriangle& plt, const Point3D& pt)
{
  return base_intersects(pt, plt);
}

bool base_intersects(const PlaneTriangle& plt, const LineSeg3D& l)
{
  return base_intersects(l, plt);
}

bool base_intersects(const PlaneTriangle& plt1, const PlaneTriangle& plt2)
{
  return intersect_triangles(plt1.triangle, plt2.triangle, plt1.plane, plt2.plane);
}

}