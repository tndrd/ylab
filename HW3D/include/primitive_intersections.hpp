#pragma once

#include "Plane3D.hpp"
#include "LineInf3D.hpp"
#include "LineSeg3D.hpp"

namespace HW3D
{

// Info about relationship between two lines
// Also contains intersection params (if exist)
// Is returned by get_line_relation() function
// This class is well-incapsulated, user won't ocassionaly change it
class LineRelation final
{
  public:
  enum State
  {
    COINCIDENT,
    PARALLEL,
    INTERSECTING,
    NON_INTERSECTING 
  };

  LineRelation(State state, double t1=NAN, double t2=NAN) noexcept:
  state_(state), t1_(t1), t2_(t2) {}

  private:
  State state_;

  double t1_;
  double t2_;
  
  public:
  State  get_state() const noexcept { return state_; }
  double get_t1()    const noexcept { return t1_; }
  double get_t2()    const noexcept { return t2_; }
};


// Info about relationship between two planes
// Also contains intersection params (if exist)
// Is returned by get_plane_relation() function
// This class is well-incapsulated, user won't ocassionaly change it
class PlaneRelation final
{
  public:
  enum State
  {
    COINCIDENT,
    PARALLEL,
    INTERSECTING
  };

  PlaneRelation(State state, double a=NAN, double b=NAN) noexcept:
  state_(state), a_(a), b_(b) {}

  private:
  State state_;

  double a_;
  double b_;

  public:
  State  get_state() const noexcept { return state_; }
  double get_a()     const noexcept { return a_; }
  double get_b()     const noexcept { return b_; }
};

// Info about relationship between line and plane
// Also contains intersection params (if exist)
// Is returned by get_relation() function
// This class is well-incapsulated, user won't ocassionaly change it
class LinePlaneRelation final
{
  public:
  enum State
  {
    COINCIDENT,
    PARALLEL,
    INTERSECTING,
    NON_INTERSECTING 
  };

  LinePlaneRelation(State state, double t=NAN) noexcept:
  state_(state), t_(t) {}

  private:
  State state_;

  double t_;
  
  public:
  State  get_state() const noexcept { return state_; }
  double get_t()    const noexcept { return t_; }
};

// Check the lines relationship (coincident, parallel, intersecting, non-intersecting)
// If lines are not coincident or parallel then check for virtual intersection [Reference: GCT page 409]
// Check the parameter restrictions (t=any for inf.line, 0<t<1 for line segment and t>0 for ray).
// If the parameter t fits both lines' restrictions, then they really intersect
// Otherwise they do not
// Returns relationship info and intersection params (if exist)
template<typename LineT1, typename LineT2>
inline LineRelation get_line_relation(const LineT1& l1, const LineT2& l2) noexcept
{
  using state_t = LineRelation;

  if (l1.is_coincident(l2))
    return {state_t::COINCIDENT};

  if (l1.is_parallel(l2))
    return {state_t::PARALLEL};

  const Vec3D   a1 = l1.get_a();
  const Point3D p1 = l1.get_p();

  const Vec3D   a2 = l2.get_a();
  const Point3D p2 = l2.get_p();

  const Vec3D  u = p1 - p2;
  const double A = a1 * a1;
  const double B = a1 * a2;
  const double C = a2 * a2;
  const double D = a1 * u;
  const double E = a2 * u;
  const double F = u * u;

  double denominator = (A*C - B*B);

  double t1 =  (B*E - C*D) / denominator; // Zero-division is not possible, 
  double t2 = -(B*D - A*E) / denominator; // lines are not parallel

  if (!l1.check_param(t1) || !l2.check_param(t2))
    return {state_t::NON_INTERSECTING, t1, t2};

  Point3D Q1 = l1.point_from_param(t1);
  Point3D Q2 = l2.point_from_param(t2);

  if (Q2 == Q1)
  {
    return {state_t::INTERSECTING, t1, t2};
  }
  else
  {
    return {state_t::NON_INTERSECTING, t1, t2};
  }
}

// Check the lines relationship (coincident, parallel, intersecting, non-intersecting)
// If lines are not coincident or parallel then check for virtual intersection [Reference: GCT page 409]
// Check the parameter restrictions (t=any for inf.line, 0<t<1 for line segment and t>0 for ray).
// If the parameter t fits both lines' restrictions, then they really intersect
// Otherwise they do not
// Returns relationship info and intersection params (if exist)
template<typename LineT>
inline LinePlaneRelation get_line_plane_relation(const LineT& l, const Plane3D& pl) noexcept
{
  using state_t = LinePlaneRelation;

  Point3D p = l.get_p();
  Vec3D   a = l.get_a();

  Vec3D  n = pl.get_n();
  double s = pl.get_s();

  if (fit(a * n, 0))
  {
    if (fit(p * n, s))
    {
      return {state_t::COINCIDENT};
    }
    else
    {
      return {state_t::PARALLEL};
    }
  }

  double t = (s - (n * p)) / (n * a);

  if (l.check_param(t))
  {
    return {state_t::INTERSECTING, t};
  }
  else
  {
    return {state_t::NON_INTERSECTING};
  } 
}

// Check the planes relationship (coincident, parallel or intersecting)
// If planes are not coincident or parallel then they do intersect with each other
// We can find the intersection line with som formulas [Reference: GCT page 530]
// Returns relationship info and intersection params (if exist)
template<typename PlaneT1, typename PlaneT2>
inline PlaneRelation get_plane_relation(const PlaneT1& p1, const PlaneT2& p2) noexcept
{
  using state_t = PlaneRelation;

  if (p1.is_coincident(p2))
    return {state_t::COINCIDENT};

  if (p1.is_parallel(p2))
    return {state_t::PARALLEL};

  Vec3D  n1 = p1.get_n();
  double s1 = p1.get_s();

  Vec3D  n2 = p2.get_n();
  double s2 = p2.get_s();

  double a1  = s2 * (n1 * n2) - s1 * (n2 * n2);
  double b1  = s1 * (n1 * n2) - s2 * (n1 * n1);

  double den = (n1 * n2) * (n1 * n2) - (n1 * n1) * (n2 * n2); // Zero-division error is not possible,
                                                              // planes are not parallel

  return {state_t::INTERSECTING, a1 / den, b1 / den};
}

// Returns the intersection point of two lines. Given lines should really intersect
template<typename LineT1, typename LineT2>
inline Point3D get_line_intersection(const LineT1& l1, const LineT2& l2, const LineRelation& relation)
{
  if (relation.get_state() != relation.INTERSECTING)
    throw std::invalid_argument("Attempt to intersect non-intersecting lines");
  
  return l1.point_from_param(relation.get_t1());
}

template<typename LineT>
inline Point3D get_line_plane_intersection(const LineT& l, const Plane3D& pl, const LinePlaneRelation& relation)
{
  if (relation.get_state() != relation.INTERSECTING)
    throw std::invalid_argument("Attempt to intersect non-intersecting lines");
  
  return l.point_from_param(relation.get_t());
}

// Returns the intersection line of two planes. Given planes should really intersect
template<typename PlaneT1, typename PlaneT2>
inline LineInf3D get_plane_intersection(const PlaneT1& p1, const PlaneT2& p2, const PlaneRelation& relation)
{
  if (relation.get_state() != relation.INTERSECTING)
    throw std::invalid_argument("Attempt to intersect non-intersecting planes");

  Vec3D n1 = p1.get_n();
  Vec3D n2 = p2.get_n();

  Vec3D a   = vecmul(n1, n2);
  Point3D p = relation.get_a() * n1 + relation.get_b() * n2;

  return {a, p}; 
}

}