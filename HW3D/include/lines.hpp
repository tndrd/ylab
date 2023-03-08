/* Contains 3D line classes */

#pragma once
#include "Vec3D.hpp" 

namespace HW3D
{

// ------ LINE CLASSES ------

class Line3D
{
  // Parent class for 3D lines
  
  protected:
  // Line equation: V = p + a*t
  Vec3D   a_; // Direction vector.
  Point3D p_; // Line offset radius-vector.

  // a and p are class invariants, so they should be protected
  // Line3D will have three childs: line segment LineSeg3D, infinite line LineInf3D, *optional* ray LineRay3D
  // They construct differently: 
  //  - LineSeg3D from two points
  //  - LineInf3D as an intersection of two planes or by explicitly given a and p
  //  - LineRay3D from direction a and point p
  // Defining a and p correctly is crucial for LineSeg3D, 
  // invalid definition will break line segment intersection algorithm
  // So there is no need for user to change a or p

  // The constructor is protected too
  // User needs to create one of childs to create a line

  Line3D(const Vec3D& a, const Point3D& p): a_(a), p_(p)
  {
    assert(!is_zero(a_));
  }

  public:

  Vec3D   get_a() const noexcept {return a_;}
  Point3D get_p() const noexcept {return p_;}

  Point3D point_from_param(data_t t) const noexcept
  {
    return p_ + (a_ * t);
  }

  bool is_parallel(const Line3D& l2) const
  {
    return abseq(get_a().normalize(), l2.get_a().normalize());
  }

  // Check if given parameter fits the restrictions on t
  virtual bool check_param(data_t /*t*/) const noexcept = 0;

  // virtual dtor
  virtual ~Line3D() { } 
};


// Infinite line class
// Can be constructed with a and p given explicitly,
// There are no restrictions on parameter t
class LineInf3D final: public Line3D
{
  public:
  LineInf3D(const Vec3D& a, const Point3D& p={0, 0, 0}): Line3D(a, p) { }

  bool check_param(data_t /*t*/) const noexcept override { return true; }
};

// Line segment class
// Can be constructed with start and end points (p1,p2)
// We define the line equation [v = p + at] as v = p1 + (p2 - p1)t
// This way only points that are defined with 0<t<1 belong to this segment
class LineSeg3D final: public Line3D
{
  public:
  LineSeg3D(const Point3D& p1, const Point3D& p2): Line3D(p2 - p1, p1) {}

  bool check_param(data_t t) const noexcept override
  {
    return interval_fit(t, 0, 1); 
  }
};

// -----------------------------------
// ------ LINE FUNCTIONS -------------

inline std::ostream& operator<<(std::ostream& os, const Line3D& line)
{
  os << "a: " << line.get_a() << " p:" << line.get_p();
  return os;
}

inline bool parallel(const Line3D& l1, const Line3D& l2) noexcept
{
  return collinear(l1.get_a(), l2.get_a());
}

// Returns vector that is orthagonal to line and fits the point
inline Vec3D distvec(const Point3D& point, const Line3D& line) noexcept
{
  Vec3D  p = line.get_p();
  Vec3D  a = line.get_a();
  
  return p + project_v(point - p, a) - point;
}

// Returns distance from point to line
inline data_t dist(const Point3D& point, const Line3D& line) noexcept
{
  return distvec(point, line).length();
}

}