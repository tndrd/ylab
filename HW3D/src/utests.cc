#include <gtest/gtest.h>
#include "task_algorithm.hpp"

using namespace HW3D;

#define EXPECT_FIT(a, b) EXPECT_TRUE(fit((a) , (b)))

//--------------------------------------------------------------------

TEST(Vec3D, SimpleOperations)
{
  Vec3D a {1, 2, 5};
  Vec3D b {5, 2, 3};
  Vec3D c {12, 8, 16};
  Vec3D r {0, 0, 0};
  EXPECT_EQ(a + b - c / 2, r);

  Vec3D x {0, 0, 0};
  Vec3D y {0, 0, 1};

  EXPECT_FALSE(x == y);
}

TEST(Vec3D, ExtraOperations)
{
  Vec3D a = {3, 4, 5};
  const double real_len = 7.07106781;
  EXPECT_FIT(a.length(), real_len);

  Vec3D b = {3 / real_len, 4 / real_len, 5 / real_len};
  EXPECT_EQ(b, a.normalize());
}

TEST(Vec3D, VectorOperations)
{
  Vec3D a = {3, 4, 5};
  Vec3D b = {5, 4, 6};
  Vec3D c = {15, 16, 30};
  EXPECT_EQ(a * b, 15 + 16 + 30);

  Vec3D v1 = {3, 3, 3};
  Vec3D a1 = {1, 1, 1};
  EXPECT_EQ(project_v(v1, a1), v1);

  Vec3D v2 = {0, 0, 1};
  Vec3D a2 = {1, 1, 0};
  Vec3D z2 = {0, 0, 0};
  EXPECT_EQ(project_v(v2, a2), z2);

  Vec3D x = {1, 0, 0};
  Vec3D y = {0, 1, 0};
  Vec3D z = {0, 0, 1};

  EXPECT_EQ(vecmul(x, y), -vecmul(y, x));
  EXPECT_EQ(vecmul(x, y), z);
  EXPECT_EQ(vecmul(y, z), x);
  EXPECT_EQ(vecmul(z, x), y);

  EXPECT_FIT(det(x, y, z), 1);
  EXPECT_FIT(det(y * 6, x, z), -6);

  Vec3D e = {1, 1, 1};
  EXPECT_FIT(det(e, e, e), 0);

  Vec3D xo = {1, 2, 3};
  Vec3D yo = {4, 5, 6};
  Vec3D zo = {7, 8, 9};
  EXPECT_FIT(det(xo, yo, zo), 0);
}

//--------------------------------------------------------------------

TEST(Lines, LineSeg3D)
{
  Point3D p1 {1, 1, 1};
  Point3D p2 {0, 0, 0};

  Point3D p3 {1, 1, 2};
  Point3D p4 {0, 0, 1};

  Point3D p5 {2, 2, 8};
  Point3D p6 {9, 1, 1};

  LineSeg3D seg1 {p2, p1};
  LineSeg3D seg2 {p1, p2};
  LineSeg3D seg3 {p3, p4};
  LineSeg3D seg4 {p5, p6};

  EXPECT_TRUE(seg1.is_coincident(seg1));
  EXPECT_TRUE(seg1.is_parallel(seg1));

  EXPECT_TRUE(seg1.is_coincident(seg2));
  EXPECT_TRUE(seg2.is_coincident(seg1));

  EXPECT_TRUE(seg1.is_parallel(seg3));
  EXPECT_TRUE(seg3.is_parallel(seg1));
  
  EXPECT_FALSE(seg1.is_coincident(seg3));
  EXPECT_FALSE(seg3.is_coincident(seg1));

  EXPECT_FALSE(seg1.is_coincident(seg4));
  EXPECT_FALSE(seg4.is_coincident(seg1));

  EXPECT_FALSE(seg1.is_parallel(seg4));
  EXPECT_FALSE(seg4.is_parallel(seg1));
}

TEST(Lines, LineInf3D)
{

  Point3D   p1 {1, 1, 1};
  Vec3D     a1 {1, 1, 1};
  LineInf3D l1 {a1, p1};

  Point3D   p2 {16, 16, 16};
  Vec3D     a2 {-1, -1, -1};
  LineInf3D l2 {a2, p2};

  Point3D   p3 {5, 4, 3};
  Vec3D     a3 {0.001, 0.001, 0.001};
  LineInf3D l3 {a3, p3};

  Point3D   p4 {9, 1, 1};
  Vec3D     a4 {2, 2, 8};
  LineInf3D l4 {a4, p4};

  Point3D   p5 {1, 1, 1};
  Vec3D     a5 {1, 1, -1};
  LineInf3D l5 {a1, p1};

  EXPECT_TRUE(l1.is_coincident(l1));
  EXPECT_TRUE(l1.is_parallel(l1));

  EXPECT_TRUE(l1.is_coincident(l2));
  EXPECT_TRUE(l2.is_coincident(l1));

  EXPECT_TRUE(l1.is_parallel(l3));
  EXPECT_TRUE(l3.is_parallel(l1));

  EXPECT_TRUE(l2.is_parallel(l3));
  EXPECT_TRUE(l3.is_parallel(l2));

  EXPECT_FALSE(l1.is_coincident(l3));
  EXPECT_FALSE(l3.is_coincident(l1));

  EXPECT_FALSE(l2.is_coincident(l3));
  EXPECT_FALSE(l3.is_coincident(l2));

  EXPECT_FALSE(l1.is_coincident(l4));
  EXPECT_FALSE(l2.is_coincident(l4));
  EXPECT_FALSE(l3.is_coincident(l4));

  EXPECT_FALSE(l1.is_parallel(l4));
  EXPECT_FALSE(l2.is_parallel(l4));
  EXPECT_FALSE(l3.is_parallel(l4));

  EXPECT_FALSE(l1.is_parallel(l4));
  EXPECT_FALSE(l1.is_coincident(l4));
}

TEST(Lines, LineMIX3D)
{
  Point3D   p1 {1, 1, 1};
  Vec3D     a1 {1, 1, 1};
  LineInf3D l1 {a1, p1};

  Point3D ps1 {0, 0, 0};
  Point3D ps2 {-3, -3, -3};
  LineSeg3D ls1 {ps1, ps2};

  Point3D   p2 {0, 0, 1};
  Vec3D     a2 {1, 1, 1};
  LineInf3D l2 {a2, p2};

  Point3D   p3 {2, 2, 8};
  Vec3D     a3 {9, 1, 1};
  LineInf3D l3 {a3, p3};

  EXPECT_TRUE(l1.is_coincident(ls1));
  EXPECT_TRUE(l1.is_parallel(ls1));

  EXPECT_TRUE(ls1.is_coincident(l1));
  EXPECT_TRUE(ls1.is_parallel(l1));

  EXPECT_FALSE(l2.is_coincident(ls1));
  EXPECT_TRUE(l2.is_parallel(ls1));

  EXPECT_FALSE(ls1.is_coincident(l2));
  EXPECT_TRUE(ls1.is_parallel(l2));

  EXPECT_FALSE(l3.is_coincident(ls1));
  EXPECT_FALSE(l3.is_parallel(ls1));

  EXPECT_FALSE(ls1.is_coincident(l3));
  EXPECT_FALSE(ls1.is_parallel(l3));
}

//--------------------------------------------------------------------

TEST(Plane3D, General)
{
  Plane3D pl1 {{1, 1, 1}, 1};
  Plane3D pl2 {{-2, -2, -2}, -2};

  EXPECT_TRUE(pl1.is_parallel(pl2));
  EXPECT_TRUE(pl1.is_coincident(pl2));
  EXPECT_TRUE(pl2.is_coincident(pl1)); 

  Plane3D pl3 {{-1, -1, -1}, 1};
  EXPECT_TRUE(pl3.is_parallel(pl2));
  EXPECT_TRUE(pl2.is_parallel(pl3));
  EXPECT_FALSE(pl3.is_coincident(pl2));
  EXPECT_FALSE(pl2.is_coincident(pl3));

  Plane3D pl4 {{1, 1, 1}, 3};
  EXPECT_FALSE(pl1.is_coincident(pl4));
  EXPECT_TRUE(pl1.is_parallel(pl4));

  Plane3D pl5 {{2, 2, -2}, 1};
  EXPECT_FALSE(pl1.is_coincident(pl5));
  EXPECT_FALSE(pl1.is_parallel(pl5));

  Plane3D pl6 {{1, -1, 1}, 0};
  EXPECT_FALSE(pl1.is_coincident(pl6));
  EXPECT_FALSE(pl1.is_parallel(pl6));

  Plane3D pl7{{-1, 1, -1}, 0};
  EXPECT_TRUE(pl6.is_coincident(pl7));
  EXPECT_TRUE(pl6.is_parallel(pl7));

  Plane3D pl8{{1,-1, 1}, 1};
  EXPECT_EQ(pl8.get_s(), pl1.get_s());
  EXPECT_FALSE(pl1.is_coincident(pl8));
  EXPECT_FALSE(pl1.is_parallel(pl8));
}

TEST(Plane3D, ThreePointsGeneral)
{
  Point3D p1 {1, 1, 2};
  Point3D p2 {1, -1, 2};
  Point3D p3 {-1, -1, 2};
  Point3D p4 {-2, -2, 2};

  Plane3D pl1 {p1, p2, p3};
  Vec3D n1 {0, 0, 1};
  double s1 = 2;
  EXPECT_EQ(pl1.get_n(), n1);
  EXPECT_EQ(pl1.get_s(), s1);
  
  Plane3D pl2 {p2, p3, p4};
  EXPECT_TRUE(pl1.is_coincident(pl2));
  EXPECT_TRUE(pl1.is_parallel(pl2));

  Vec3D n3 {0, 0, -2};
  double s3 = -4;
  Plane3D pl3 {n3, s3};

  EXPECT_EQ(pl3.get_n(), n1);
  EXPECT_EQ(pl3.get_s(), s1);
}

TEST(Plane3D, ThreePoints1)
{
  Point3D p1 {-2, 0, 0};
  Point3D p2 {0, -2, 0};
  Point3D p3 {0, 0, -2};

  Plane3D pl {p1, p2, p3};
  double a = -1 / std::sqrt(3) ;
  Vec3D  n {a, a, a};
  double s = 2 / std::sqrt(3); 

  EXPECT_EQ(pl.get_n(), n);
  EXPECT_EQ(pl.get_s(), s);
}

TEST(Plane3D, ThreePoints2)
{
  Point3D p1 {0, 0, 0};
  Point3D p2 {0, 5, 0};
  Point3D p3 {0, 0, 5};

  Plane3D pl {p1, p2, p3};
  Vec3D  n {-1, 0, 0};
  double s = 0; 

  EXPECT_EQ(pl.get_n(), n);
  EXPECT_EQ(pl.get_s(), s);
}

TEST(Plane3D, ThreePoints3)
{
  Point3D p1 {-4, 8, 5};
  Point3D p2 {-3, 6, -1};
  Point3D p3 {7, 8, 3};

  Plane3D pl {p1, p2, p3};
  Vec3D  n {-0.0590024, 0.944039, -0.324513};
  double s = 6.16576; 

  EXPECT_EQ(pl.get_n(), n);
  EXPECT_FIT(pl.get_s(), s);
}

TEST(Plane3D, ThreePoints4)
{
  Point3D p1 {7, -8, 7};
  Point3D p2 {-7, -1, 4};
  Point3D p3 {1, -4, -1};

  Plane3D pl {p1, p2, p3};
  Vec3D  n {-0.420135, -0.897561, -0.133679};
  double s = 3.30379; 

  EXPECT_EQ(pl.get_n(), n);
  EXPECT_FIT(pl.get_s(), s);
}

TEST(Plane3D, ThreePoints5)
{
  Point3D p1 {-7, 8, 7};
  Point3D p2 {-7, -1, 4};
  Point3D p3 {0, 0, 0};

  Plane3D pl {p1, p2, p3};
  Vec3D  n {-0.506408, 0.272681, -0.818044};
  double s = 0; 

  EXPECT_EQ(pl.get_n(), n);
  EXPECT_EQ(pl.get_s(), s);
}

//--------------------------------------------------------------------

using state_t = LineRelation::State;

TEST(LineIntersections, LineInf_LineInf1)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineInf3D a {(intr - pa) * 8,  pa};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);
}

TEST(LineIntersections, LineInf_LineInf2)
{
  LineInf3D a {{5, 3, 2}, {-10, -6, -4}};
  LineInf3D b {{4, 4, 4}, {-10, -10, -10}};
  Point3D intr {0, 0, 0};

  LineRelation rel1 = get_relation(a, b);
  ASSERT_EQ(rel1.get_state(), state_t::INTERSECTING);

  Point3D intr_c1 = get_intersection(a, b, rel1);
  EXPECT_EQ(intr, intr_c1);

  LineRelation rel2 = get_relation(b, a);
  ASSERT_EQ(rel2.get_state(), state_t::INTERSECTING);

  Point3D intr_c2 = get_intersection(b, a, rel2);
  EXPECT_EQ(intr, intr_c2);
}

TEST(LineIntersections, LineInf_LineInf3)
{
  LineInf3D a {{1, 1, 1}, {0, 0, 0}};
  LineInf3D b {{-1, -1, -1}, {0, 0, 5}};
  LineInf3D c {{5, 3, -5}, {1, 1, 1}};
  LineInf3D d {{-5, -5, -5}, {0.1, 0.1, 0.1}};

  LineRelation rel1 = get_relation(a, d);
  EXPECT_EQ(rel1.get_state(), state_t::COINCIDENT);

  LineRelation rel2 = get_relation(a, b);
  EXPECT_EQ(rel2.get_state(), state_t::PARALLEL);

  LineRelation rel3 = get_relation(b, c);
  EXPECT_EQ(rel3.get_state(), state_t::NON_INTERSECTING);
}

//--------------------------------------------------------------------

TEST(LineIntersections, LineSeg_LineInf1)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {pa + (intr - pa) * 2,  pa};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);
}

TEST(LineIntersections, LineSeg_LineInf2)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {intr,  pa};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);
}

TEST(LineIntersections, LineSeg_LineInf3)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {pa, intr};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);
}

TEST(LineIntersections, LineSeg_LineInf4)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {pa + (intr - pa) * 0.5,  pa};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::NON_INTERSECTING);
}

TEST(LineIntersections, LineSeg_LineInf5)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {pa, pa + (intr - pa) * 0.5};
  LineInf3D b {(intr - pb) * -3, pb};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::NON_INTERSECTING);
}

TEST(LineIntersections, LineSeg_LineInf6)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  LineSeg3D a {pa, intr};
  LineInf3D b {(intr - pa) * -3, pa};

  LineRelation rel1 = get_relation(a, b);
  LineRelation rel2 = get_relation(b, a);

  EXPECT_EQ(rel1.get_state(), state_t::COINCIDENT);
  EXPECT_EQ(rel2.get_state(), state_t::COINCIDENT);
}

TEST(LineIntersections, LineSeg_LineInf7)
{
  Point3D intr {2, 3, 4};

  Point3D pa {-5, -3, 0};
  Point3D pb {6, 4, -1};

  Vec3D ofs {0, 0, 1};

  LineSeg3D a {pa + ofs , intr + ofs};
  LineInf3D b {intr - pa, pa};

  LineRelation rel = get_relation(a, b);

  ASSERT_EQ(rel.get_state(), state_t::PARALLEL);
}

//--------------------------------------------------------------------

TEST(LineIntersections, LineSeg_LineSeg1)
{
  Point3D intr {2, 2, 8};

  Point3D pa {3, 4, 5};
  Point3D pb {-1, -2, -3};

  LineSeg3D a {pa, pa + (intr - pa) * 2};
  LineSeg3D b {pb, pb + (intr - pb) * 2};

  LineRelation rel = get_relation(a, b);
  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);
}

TEST(LineIntersections, LineSeg_LineSeg2)
{
  Point3D intr {-1, -1, -1};

  Point3D pa {0, 0, 0};
  Point3D pb {-1, -2, -3};

  LineSeg3D a {pa, pa + (intr - pa)};
  LineSeg3D b {pb, pb + (intr - pb) * 2};

  LineRelation rel = get_relation(a, b);
  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);

  LineRelation rel2 = get_relation(b, a);
  ASSERT_EQ(rel2.get_state(), state_t::INTERSECTING);

  Point3D intr_c2 = get_intersection(b, a, rel2);
  EXPECT_EQ(intr, intr_c2);
}

TEST(LineIntersections, LineSeg_LineSeg3)
{
  Point3D intr {-2, -2, -8};

  Point3D pa {1, 1, 1};
  Point3D pb {-1, -2, -3};

  LineSeg3D a {pa, pa + (intr - pa)};
  LineSeg3D b {pb, pb + (intr - pb)};
  LineSeg3D c {pa + (intr - pa), pa};

  LineRelation rel = get_relation(a, b);
  ASSERT_EQ(rel.get_state(), state_t::INTERSECTING);

  Point3D intr_c = get_intersection(a, b, rel);
  EXPECT_EQ(intr, intr_c);

  LineRelation rel2 = get_relation(b, a);
  ASSERT_EQ(rel2.get_state(), state_t::INTERSECTING);

  Point3D intr_c2 = get_intersection(b, a, rel2);
  EXPECT_EQ(intr, intr_c2);

  LineRelation rel3 = get_relation(b, c);
  ASSERT_EQ(rel3.get_state(), state_t::INTERSECTING);

  Point3D intr_c3 = get_intersection(b, c, rel3);
  EXPECT_EQ(intr, intr_c3);
}

TEST(LineIntersections, LineSeg_LineSeg4)
{
  Point3D intr {-2, -2, -8};

  Point3D pa {1, 1, 1};
  Point3D pb {-1, -2, -3};

  LineSeg3D a {pa, pa + (intr - pa) * 0.5};
  LineSeg3D b {pb, pb + (intr - pb)};

  LineRelation rel = get_relation(a, b);
  EXPECT_EQ(rel.get_state(), state_t::NON_INTERSECTING);

  LineRelation rel2 = get_relation(b, a);
  EXPECT_EQ(rel2.get_state(), state_t::NON_INTERSECTING);
}

TEST(LineIntersections, LineSeg_LineSeg5)
{
  Point3D pa {1, 1, 1};
  Point3D pb {-1, -2, -3};
  Vec3D ofs {0, 0, 1};

  LineSeg3D a {pa, pb};
  LineSeg3D b {pb, pa};
  LineSeg3D c {pa + ofs, pb + ofs};

  LineRelation rel = get_relation(a, b);
  EXPECT_EQ(rel.get_state(), state_t::COINCIDENT);

  LineRelation rel2 = get_relation(b, c);
  EXPECT_EQ(rel2.get_state(), state_t::PARALLEL);
}

TEST(LineIntersections, LineSeg_LineSeg6)
{
  Point3D pa {1, 1, 1};
  Point3D pb {-1, -2, -3};
  Point3D pc {3, 2, 3};
  Point3D pd {8, 8, 2};

  LineSeg3D a {pa, pb};
  LineSeg3D b {pc, pd};

  LineRelation rel = get_relation(a, b);
  EXPECT_EQ(rel.get_state(), state_t::NON_INTERSECTING);
}