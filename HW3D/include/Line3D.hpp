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

  Line3D(const Vec3D& a, const Point3D& p): a_(a), p_(p) {}

  public:

  Vec3D   get_a() const {return a_;}
  Point3D get_p() const {return p_;}

  Point3D point_from_param(double t) const
  {
    return p_ + (a_ * t);
  }

  double param_from_point(const Point3D& p) const
  {
    double t = ((p - p_) * a_) / (a_ * a_);

    if (!(p == point_from_param(t))) throw;

    return t;
  }

  bool is_parallel(const Line3D& l2) const
  {
    return abseq(a_.normalize(), l2.a_.normalize());
  }

  // Coincidence of childs determines differently for each child
  // virtual bool is_coincident(const Line3D&) const { throw; };

  // Check if given parameter fits the restrictions on t
  // virtual bool check_param(double t) const { throw; };  
};

}