#pragma once

#include "Vec3D.hpp"
#include "lines.hpp"
#include "Triangle3D.hpp"
#include "triangle_intersections.hpp"
#include <typeinfo>
#include <memory>

namespace HW3D
{

struct IntersectiblePoint;
struct IntersectibleLineSeg;
struct IntersectibleTriangle;

inline bool base_intersects(const IntersectiblePoint& pt1, const IntersectiblePoint& pt2);
inline bool base_intersects(const IntersectiblePoint& pt, const IntersectibleLineSeg& l);
inline bool base_intersects(const IntersectiblePoint& pt, const IntersectibleTriangle& tr);
inline bool base_intersects(const IntersectibleLineSeg& l, const IntersectiblePoint& pt);
inline bool base_intersects(const IntersectibleLineSeg& l1, const IntersectibleLineSeg& l2);
inline bool base_intersects(const IntersectibleLineSeg& l, const IntersectibleTriangle& tr);
inline bool base_intersects(const IntersectibleTriangle& tr, const IntersectiblePoint& pt);
inline bool base_intersects(const IntersectibleTriangle& tr, const IntersectibleLineSeg& l);
inline bool base_intersects(const IntersectibleTriangle& tr1, const IntersectibleTriangle& tr2);


struct IntersectibleWrapper
{
  virtual ~IntersectibleWrapper() { }

  virtual bool intersects(const IntersectiblePoint& pt) const = 0;
  virtual bool intersects(const IntersectibleLineSeg& l) const = 0;
  virtual bool intersects(const IntersectibleTriangle& tr) const = 0;
};

struct IntersectiblePoint final: public IntersectibleWrapper 
{
  Point3D point;
  IntersectiblePoint(const Point3D& p): point(p) { }

  bool intersects(const IntersectiblePoint& pt) const override
  {
    return base_intersects(*this, pt);
  }

  bool intersects(const IntersectibleLineSeg& l) const override
  {
    return base_intersects(*this, l);
  }

  bool intersects(const IntersectibleTriangle& tr) const override
  {
    return base_intersects(*this, tr);
  }
};

struct IntersectibleLineSeg final: public IntersectibleWrapper
{
  LineSeg3D line;
  IntersectibleLineSeg(const Point3D& p1, const Point3D& p2): line(p1, p2) { }

  bool intersects(const IntersectiblePoint& pt) const override
  {
    return base_intersects(*this, pt);
  }

  bool intersects(const IntersectibleLineSeg& l) const override
  {
    return base_intersects(*this, l);
  }

  bool intersects(const IntersectibleTriangle& tr) const override
  {
    return base_intersects(*this, tr);
  }
};

struct IntersectibleTriangle final: public IntersectibleWrapper
{
  Triangle3D triangle;
  Plane3D    plane;

  IntersectibleTriangle(const Point3D& p1, const Point3D& p2, const Point3D& p3):
  triangle(p1, p2, p3), plane(p1, p2, p3) { }

  bool intersects(const IntersectiblePoint& pt) const override
  {
    return base_intersects(*this, pt);
  }

  bool intersects(const IntersectibleLineSeg& l) const override
  {
    return base_intersects(*this, l);
  }

  bool intersects(const IntersectibleTriangle& tr) const override
  {
    return base_intersects(*this, tr);
  }
};

inline bool base_intersects(const IntersectiblePoint& pt1, const IntersectiblePoint& pt2)
{
  return pt1.point == pt2.point;
}

inline bool base_intersects(const IntersectiblePoint& pt, const IntersectibleLineSeg& l)
{
  Vec3D p = l.line.get_p();
  Vec3D a = l.line.get_a();
  if (pt.point == p) 
  {
    return true;
  }

  Vec3D direction = (pt.point - p);

  if (!(direction.normalize() == a.normalize()))
  {
    return false;
  }
  data_t t2 = (direction * direction) / (a * a);
  return l.line.check_param(std::sqrt(t2));
}

inline bool base_intersects(const IntersectiblePoint& pt, const IntersectibleTriangle& tr)
{
  Plane3D plane = tr.plane;

  data_t s0 = pt.point * plane.get_n();
  data_t s  = plane.get_s();
  if (!fit(s0, s)) return false;
  
  auto normals = get_edge_normals(tr.triangle);
  for (int i = 0; i < 3; ++i)
  {
    auto interval = ComputeInterval(tr.triangle, normals[i]);
    
    data_t min = interval[0];
    data_t max = interval[1];
    
    data_t t = pt.point * normals[i];
    if (!interval_fit(t, min, max)) return false;
  }
  return true;
}

inline bool base_intersects(const IntersectibleLineSeg& l, const IntersectiblePoint& pt)
{
  return base_intersects(pt, l); 
}

inline bool base_intersects(const IntersectibleLineSeg& l1, const IntersectibleLineSeg& l2)
{
  LineRelation rel = get_line_relation(l1.line, l2.line);
  LineRelation::State state = rel.get_state();

  switch(state)
  {
    case LineRelation::NON_INTERSECTING: return false;
    
    case LineRelation::INTERSECTING: return true;
    
    case LineRelation::PARALLEL:
    {
      IntersectiblePoint p1 = l1.line.get_p();
      IntersectiblePoint p2 = p1.point + l1.line.get_a();
      return base_intersects(p1, l2) || base_intersects(p2, l2);   
    }
  }
}

inline bool base_intersects(const IntersectibleLineSeg& l, const IntersectibleTriangle& tr)
{
  Plane3D plane = tr.plane;
  LinePlaneRelation rel = get_line_plane_relation(l.line, plane);

  if (rel.get_state() == LinePlaneRelation::NON_INTERSECTING || 
      rel.get_state() == LinePlaneRelation::PARALLEL) return false;

  if (rel.get_state() == LinePlaneRelation::COINCIDENT)
  {
    std::vector<data_t> intersections = intersect_with(tr.triangle, l.line);
    return intersections.size() > 0;
  }

  if (rel.get_state() == LinePlaneRelation::INTERSECTING)
  {
    IntersectiblePoint pt = get_line_plane_intersection(l.line, plane, rel);
    return base_intersects(pt, tr);
  }
  return false;
}

inline bool base_intersects(const IntersectibleTriangle& tr, const IntersectiblePoint& pt)
{
  return base_intersects(pt, tr);
}

inline bool base_intersects(const IntersectibleTriangle& tr, const IntersectibleLineSeg& l)
{
  return base_intersects(l, tr);
}

inline bool base_intersects(const IntersectibleTriangle& tr1, const IntersectibleTriangle& tr2)
{
  return intersect_triangles(tr1.triangle, tr2.triangle, tr1.plane, tr2.plane);
}

class IntersectibleFactory
{ 
  private: 
  
  static std::unique_ptr<IntersectibleWrapper> create_triangle(const std::vector<Point3D>& pts)
  {
    assert(pts.size() == 3);
    return std::make_unique<IntersectibleTriangle>(pts[0], pts[1], pts[2]);
  }

  static std::unique_ptr<IntersectibleWrapper> create_lineseg(const std::vector<Point3D>& pts)
  {
    assert(pts.size() == 2);
    return std::make_unique<IntersectibleLineSeg>(pts[0], pts[1]);
  }

  static std::unique_ptr<IntersectibleWrapper> create_point(const std::vector<Point3D>& pts)
  {
    assert(pts.size() == 1);
    return std::make_unique<IntersectiblePoint>(pts[0]);
  }

  static std::vector<Point3D> get_unique_pts(const std::array<Point3D, 3>& pts)
  {
    std::vector<Point3D> unique_pts;
    for (int i = 0; i < 3; ++i)
    {
      Point3D point = pts[i];
      bool is_unique = true;
      for (size_t k = 0; k < unique_pts.size(); ++k)
        if (point == unique_pts[k])
        {
          is_unique = false;
          break;
        }
      if (is_unique)
        unique_pts.push_back(point);
    }
    return unique_pts;
  }

  static std::vector<Point3D> get_noncollinear_pts(const std::vector<Point3D>& unique_pts)
  {
    assert(unique_pts.size() == 3);

    for (int i = 0; i < 3; ++i)
    {
      Vec3D edge      = unique_pts[(i + 1) % 3] - unique_pts[i];
      Vec3D direction = unique_pts[(i + 2) % 3] - unique_pts[i];

      if (edge.normalize() == direction.normalize())
      {
        data_t t2 = (direction * direction) / (edge * edge);
        if (interval_fit(std::sqrt(t2), 0, 1))
        {
          return {unique_pts[i], unique_pts[(i + 1) % 3]};
        }
      }
    }

    return unique_pts;
  }

  static std::vector<Point3D> get_undegenerate_points(const std::array<Point3D, 3>& pts)
  {
    std::vector<Point3D> unique_pts = get_unique_pts(pts);
    if (unique_pts.size() < 3) return unique_pts;

    return get_noncollinear_pts(unique_pts);
  }

  public:

  static std::unique_ptr<IntersectibleWrapper> create(std::array<Point3D, 3>& pts)
  {
    std::vector<Point3D> undegenerate_pts = get_undegenerate_points(pts);

    switch(undegenerate_pts.size())
    {
      case 1: return create_point(undegenerate_pts);
      case 2: return create_lineseg(undegenerate_pts);
      case 3: return create_triangle(undegenerate_pts);
    }
  }
};

inline bool intersects(const IntersectibleWrapper& lhs, const IntersectibleWrapper& rhs)
{
  if (typeid(rhs) == typeid(IntersectiblePoint&))    return lhs.intersects(static_cast<const IntersectiblePoint&>(rhs));
  if (typeid(rhs) == typeid(IntersectibleLineSeg&))  return lhs.intersects(static_cast<const IntersectibleLineSeg&>(rhs));
  if (typeid(rhs) == typeid(IntersectibleTriangle&)) return lhs.intersects(static_cast<const IntersectibleTriangle&>(rhs));
}

}