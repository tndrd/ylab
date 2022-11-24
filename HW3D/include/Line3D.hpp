#include "Vec3D.hpp" 

namespace HW3D
{
class Line3D
{
  // Line equation: V = p + a*t
  Vec3D   a_; // Direction vector.
  Point3D p_; // Line offset radius-vector.

  public:

  Line3D(const Point3D& p1, const Point3D& p2): a_(p2 - p1), p_(p1) {}

  Vec3D   get_a() const { return a_; } // Getters 
  Point3D get_p() const { return p_; } //

  Point3D point_from_param(double t) const
  {
    return p_ + a_ * t;
  }
};

struct Line2LineRelation
{
  enum State
  {
    PARALLEL,
    COINCIDENT,
    INTERSECTING,
    NON_INTERSECTING,
  } state;

  // Let the distance vector between l1 and l2 be Q2 - Q1
  // Q1 = p1 + a1 * t1
  // Q2 = p2 + a2 * t2

  double t1  = NAN; 
  double t2 = NAN;
};

bool parallel(const Line3D& l1, const Line3D& l2)
{
  return abs(l1.get_a()) == abs(l2.get_a());
}

bool coincident(const Line3D& l1, const Line3D& l2)
{
  return parallel(l1, l2) && (l1.get_p() == l2.get_p());
}

// Reference: GCT page 409
Line2LineRelation get_lines_relation(const Line3D& l1, const Line3D& l2)
{

  Line2LineRelation relation;

  if (parallel(l1, l2))
  {
    relation.state = Line2LineRelation::State::PARALLEL;
    return relation;
  }

  if (coincident(l1, l2))
  {
    relation.state = Line2LineRelation::State::COINCIDENT;
    return relation;
  }

  Vec3D   a1 = l1.get_a();
  Point3D p1 = l1.get_p();

  Vec3D   a2 = l2.get_a();
  Point3D p2 = l2.get_p();

  Vec3D  u = p1 - p2;
  double A = a1 * a1;
  double B = - a1 * a2;
  double C = a2 * a2;
  double D = a1 * u;
  double E = a2 * u;
  double F = u * u;

  relation.t1 = (A*E - C*D) / (A*C - B*B);
  relation.t2 = (B*D - A*E) / (A*C - B*B);

  Point3D Q1 = l1.point_from_param(relation.t1);
  Point3D Q2 = l2.point_from_param(relation.t2);

  if (fit((Q2 - Q1).length(), 0))
  {
    relation.state = Line2LineRelation::State::INTERSECTING;
  }
  else
  {
    relation.state = Line2LineRelation::State::NON_INTERSECTING;
  }

  return relation;
}

}