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
//--------------------------------------------------------------------

TEST(Plane3D, General)
{
  Plane3D pl1 {{1, 1, 1}, 1};
  Plane3D pl2 {{-1, -1, -1}, -1};

  EXPECT_TRUE(pl1.is_parallel(pl2));
  EXPECT_TRUE(pl1.is_coincident(pl2));
  EXPECT_TRUE(pl2.is_coincident(pl1)); 

  Plane3D pl3 {{1, 1, 1}, 5};
  EXPECT_TRUE(pl3.is_parallel(pl2));
  EXPECT_TRUE(pl2.is_parallel(pl3));
  EXPECT_FALSE(pl3.is_coincident(pl2));
  EXPECT_FALSE(pl2.is_coincident(pl3));

  Plane3D pl4 {{1, 1, -1}, 1};
  EXPECT_FALSE(pl1.is_coincident(pl4));
  EXPECT_FALSE(pl1.is_parallel(pl4));

  Plane3D pl5 {{1, 0, 0}, 0};
  EXPECT_FALSE(pl1.is_coincident(pl5));
  EXPECT_FALSE(pl1.is_parallel(pl5));
}