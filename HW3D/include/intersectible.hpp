#pragma once
#include "base_intersects.hpp"
#include <memory>

namespace HW3D
{

struct IIntersectible
{
  virtual bool intersects(const IIntersectible& rhs) const = 0;
  virtual ~IIntersectible() { }

  virtual bool intersects(const Point3D&    rhs) const = 0;
  virtual bool intersects(const LineSeg3D&  rhs) const = 0;
  virtual bool intersects(const PlaneTriangle& rhs) const = 0;
};

struct IntersectiblePoint final: public IIntersectible 
{ 
  Point3D data;
  IntersectiblePoint(const Point3D& p): data(p) { }

  bool intersects(const IIntersectible& rhs) const override
  {
    return rhs.intersects(data);
  }

  bool intersects(const Point3D&       rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const LineSeg3D&     rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const PlaneTriangle& rhs) const override { return base_intersects(data, rhs); }
};

struct IntersectibleLineSeg final: public IIntersectible 
{ 
  LineSeg3D data;
  IntersectibleLineSeg(const Point3D& p1, const Point3D& p2): data(p1, p2) { }

  bool intersects(const IIntersectible& rhs) const override
  {
    return rhs.intersects(data);
  }

  bool intersects(const Point3D&       rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const LineSeg3D&     rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const PlaneTriangle& rhs) const override { return base_intersects(data, rhs); }
};

struct IntersectibleTriangle final: public IIntersectible 
{ 
  PlaneTriangle data;
  IntersectibleTriangle(const Point3D& p1, const Point3D& p2, const Point3D& p3): data(p1, p2, p3) { }

  bool intersects(const IIntersectible& rhs) const override
  {
    return rhs.intersects(data);
  }

  bool intersects(const Point3D&       rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const LineSeg3D&     rhs) const override { return base_intersects(data, rhs); }
  bool intersects(const PlaneTriangle& rhs) const override { return base_intersects(data, rhs); }
};

class IntersectibleFactory
{ 
  private: 
  
  static std::unique_ptr<IIntersectible> create_triangle(const std::vector<Point3D>& pts)
  {
    assert(pts.size() == 3);
    return std::make_unique<IntersectibleTriangle>(pts[0], pts[1], pts[2]);
  }

  static std::unique_ptr<IIntersectible> create_lineseg(const std::vector<Point3D>& pts)
  {
    assert(pts.size() == 2);
    return std::make_unique<IntersectibleLineSeg>(pts[0], pts[1]);
  }

  static std::unique_ptr<IIntersectible> create_point(const std::vector<Point3D>& pts)
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

  static std::unique_ptr<IIntersectible> create(std::array<Point3D, 3>& pts)
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

inline bool intersects(const IIntersectible& lhs, const IIntersectible& rhs)
{
  return lhs.intersects(rhs);
}

}