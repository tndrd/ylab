#include "Line3D.hpp"
#include <vector>

namespace HW3D
{

class LineSeg3D
{

  // LineSegment3D equation:
  // a = p2 - p1
  // p = p1
  // V = p + a * t; 0 <= t <= 1; 

  Point3D v1_;
  Point3D v2_;
  Line3D  line_;

  public:

  Line3D  get_line() const { return line_; } // Getters 
  Point3D get_v1()   const { return v1_; }   // 
  Point3D get_v2()   const { return v2_; }   //

  LineSeg3D(const Point3D& p1, const Point3D& p2): line_(p1, p2), v1_(p1), v2_(p2) {} 
};

bool coincident(const LineSeg3D& ls1, const LineSeg3D& ls2)
{
  bool cond1 =  ls1.get_v1() == ls2.get_v1() && ls1.get_v2() == ls2.get_v2();
  bool cond2 =  ls1.get_v1() == ls2.get_v2() && ls1.get_v2() == ls2.get_v1();
  return cond1 || cond2;
}

struct LineSeg2LineSegRelation
{
  enum State
  {
    COINCIDENT,
    // PARALLEL, // Excluded these two to simplify the algorithm :)
    // TANGENT,
    INTERSECTING,
    NON_INTERSECTING
  } state;

  // Let the distance vector between ls1 and ls2 be Q2 - Q1
  // Q1 = p1 + a1 * t1; 0 < t1 < 1
  // Q2 = p2 + a2 * t2; 0 < t2 < 1

  double t1;
  double t2;
};

LineSeg2LineSegRelation get_lineseg_relation(const LineSeg3D& ls1, const LineSeg3D& ls2)
{

  LineSeg2LineSegRelation lineseg_relation;

  // Check if coincident
  if (coincident(ls1, ls2))
  {
    lineseg_relation.state = LineSeg2LineSegRelation::State::COINCIDENT;
    return lineseg_relation;
  }

  Line2LineRelation line_relation = get_lines_relation(ls1.get_line(), ls2.get_line());

  // Check if lines intersect
  if (line_relation.state != Line2LineRelation::State::INTERSECTING)
  {
    lineseg_relation.state = LineSeg2LineSegRelation::State::NON_INTERSECTING;
    return lineseg_relation;
  }
  
  double t1 = line_relation.t1;
  double t2 = line_relation.t2;

  // Check if line segments intersect
  bool cond_t1 = (t1 > 0) && (t1 < 1);
  bool cond_t2 = (t2 > 0) && (t2 < 1);

  if (cond_t1 && cond_t2)
  {
    lineseg_relation.state = LineSeg2LineSegRelation::State::INTERSECTING;
    lineseg_relation.t1 = t1;
    lineseg_relation.t2 = t2;
    return lineseg_relation;
  }
   
  lineseg_relation.state = LineSeg2LineSegRelation::State::NON_INTERSECTING;
  return lineseg_relation;
}

}