#pragma once

#include "Vec3D.hpp" 

namespace HW3D
{

class Line3D
{
  // Parent class for 3D lines
  
  protected:
  // Line equation: V = p + a*t
  Vec3D   a_; // Direction vector.
  Point3D p_; // Line offset radius-vector.

  // a and p are class invariants, so they should be protected
  // Line3D will have three childs: line segment LineSeg3D, infinite line LineInf3D, ray LineRay3D
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
    if (a_ == Vec3D{0, 0, 0}) throw std::invalid_argument("Line direction vector can not be zero");
  }

  // Protected dtor
  ~Line3D() { }

  public:

  Vec3D   get_a() const noexcept {return a_;}
  Point3D get_p() const noexcept {return p_;}

  Point3D point_from_param(data_t t) const noexcept
  {
    return p_ + (a_ * t);
  }

  bool is_parallel(const Line3D& l2) const noexcept
  {
    return abseq(a_.normalize(), l2.a_.normalize());
  }

  // Coincidence of childs determines differently for each child
  // virtual bool is_coincident(const Line3D&) const { throw; };

  // Check if given parameter fits the restrictions on t
  // virtual bool check_param(data_t t) const { throw; };  
};

inline std::ostream& operator<<(std::ostream& os, const Line3D& l)
{
    os << "a: " << l.get_a() << " p:" << l.get_p();
    return os;
}

inline data_t dist(const Line3D& line, const Point3D& point)
{
  Point3D P = line.get_p() + project_v(point - line.get_p(), line.get_a());
  return (P - point).length();
}

}