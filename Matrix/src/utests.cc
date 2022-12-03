#include <gtest/gtest.h>
#include "matrix.hpp"

using namespace HWMatrix;

static double FIT_TOLERANCE = 0.00001; 

static bool fit(double a, double b)
{
  return std::abs(a - b) < FIT_TOLERANCE;
}

#define EXPECT_FIT(a,b) EXPECT_TRUE(fit((a), (b)))


TEST(Matrix, Swap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Matrix<double, 3, 3> mat {data};
  EXPECT_EQ(mat[0][0], 1);
  mat.swap_rows(0, 1);
  EXPECT_EQ(mat[0][0], 4);
}

TEST(Matrix, CopyCtorSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Matrix<double, 3, 3> mat {data};
  Matrix<double, 3, 3> copy1 = mat;
  
  EXPECT_EQ(copy1[0][0], 1);
  EXPECT_EQ(mat[0][0], 1);
  copy1.swap_rows(0, 1);
  EXPECT_EQ(copy1[0][0], 4);
  EXPECT_EQ(mat[0][0], 1);

  mat.swap_rows(0,2);
  EXPECT_EQ(mat[0][0], 7);
  Matrix<double, 3, 3> copy2 = mat;
  EXPECT_EQ(copy2[0][0], 7);
}

TEST(Matrix, CopyAssignSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> empt {0, 0, 0, 0, 0, 0, 0, 0, 0};
  Matrix<double, 3, 3> mat {data};
  Matrix<double, 3, 3> copy1 {empt};
  copy1.swap_rows(0,1);
  copy1 = mat;
  
  
  EXPECT_EQ(copy1[0][0], 1);
  EXPECT_EQ(mat[0][0], 1);
  copy1.swap_rows(0, 1);
  EXPECT_EQ(copy1[0][0], 4);
  EXPECT_EQ(mat[0][0], 1);

  mat.swap_rows(0,2);
  EXPECT_EQ(mat[0][0], 7);
  Matrix<double, 3, 3> copy2 {empt};
  copy2.swap_rows(0,1);
  copy2 = mat;
  EXPECT_EQ(copy2[0][0], 7);
}

TEST(Matrix, MoveCtorSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  
  Matrix<double, 3, 3> newm = Matrix<double, 3, 3>{data}.swap_rows(0,1).swap_rows(1,2);

  EXPECT_EQ(newm[0][0], 4);
  EXPECT_EQ(newm[1][0], 7);
  EXPECT_EQ(newm[2][0], 1);
}

TEST(Matrix, MoveAssignSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> empt {0, 0, 0, 0, 0, 0, 0, 0, 0};

  Matrix<double, 3, 3> newm = empt;
  newm.swap_rows(0, 1);

  newm = Matrix<double, 3, 3>{data}.swap_rows(0,1).swap_rows(1,2);

  EXPECT_EQ(newm[0][0], 4);
  EXPECT_EQ(newm[1][0], 7);
  EXPECT_EQ(newm[2][0], 1);
}

// GENERATED TESTS

TEST(Matrix, Determinant0)
{
  std::vector<double> data { 1, 0, -2, 0.5, 3, 1, 0, 2, -1 };
  Matrix<double, 3, 3> mat = data;
  EXPECT_FIT(det(mat), -7);
}

TEST(Matrix, Determinant1)
{
  std::vector<double> data  { 42.0 };
  Matrix<double, 1, 1> mat = data;
  EXPECT_FIT(det(mat), 42.0);
}


TEST(Matrix, Determinant2)
{
  std::vector<double> data  { -42.0 };
  Matrix<double, 1, 1> mat = data;
  EXPECT_FIT(det(mat), -42.0);
}


TEST(Matrix, Determinant3)
{
  std::vector<double> data  { 0.0 };
  Matrix<double, 1, 1> mat = data;
  EXPECT_FIT(det(mat), 0.0);
}


TEST(Matrix, Determinant4)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  1.0 };
  Matrix<double, 2, 2> mat = data;
  EXPECT_FIT(det(mat), 1.0);
}


TEST(Matrix, Determinant5)
{
  std::vector<double> data  { 0.0,  1.0,  1.0,  0.0 };
  Matrix<double, 2, 2> mat = data;
  EXPECT_FIT(det(mat), -1.0);
}


TEST(Matrix, Determinant6)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0 };
  Matrix<double, 2, 2> mat = data;
  EXPECT_FIT(det(mat), 0.0);
}


TEST(Matrix, Determinant7)
{
  std::vector<double> data  { 11.0,  -2.0,  7.0,  5.0 };
  Matrix<double, 2, 2> mat = data;
  EXPECT_FIT(det(mat), 69.0);
}


TEST(Matrix, Determinant8)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0,  1.0 };
  Matrix<double, 3, 3> mat = data;
  EXPECT_FIT(det(mat), 1.0);
}


TEST(Matrix, Determinant9)
{
  std::vector<double> data  { 0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,  0.0 };
  Matrix<double, 3, 3> mat = data;
  EXPECT_FIT(det(mat), 1.0);
}


TEST(Matrix, Determinant10)
{
  std::vector<double> data  { 3.0,  3.0,  -1.0,  4.0,  1.0,  3.0,  1.0,  -2.0,  -2.0 };
  Matrix<double, 3, 3> mat = data;
  EXPECT_FIT(det(mat), 54.0);
}


TEST(Matrix, Determinant11)
{
  std::vector<double> data  { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0 };
  Matrix<double, 3, 3> mat = data;
  EXPECT_FIT(det(mat), 0.0);
}


TEST(Matrix, Determinant12)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  21.0,  0.0,  0.0,  0.0,  0.0,  2.0 };
  Matrix<double, 4, 4> mat = data;
  EXPECT_FIT(det(mat), 42.0);
}


TEST(Matrix, Determinant13)
{
  std::vector<double> data  { -2.0,  1.0,  3.0,  2.0,  3.0,  0.0,  -1.0,  2.0,  -5.0,  2.0,  3.0,  0.0,  4.0,  -1.0,  2.0,  -3.0 };
  Matrix<double, 4, 4> mat = data;
  EXPECT_FIT(det(mat), -80.0);
}


TEST(Matrix, Determinant14)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  -2.0,  0.0,  1.0,  5.0,  4.0,  3.0,  1.0,  1.0,  0.0,  -1.0,  0.0,  2.0,  2.0 };
  Matrix<double, 4, 4> mat = data;
  EXPECT_FIT(det(mat), -12.0);
}


TEST(Matrix, Determinant15)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  0.0,  0.0,  1.0,  1.0,  5.0,  3.0,  1.0,  7.0,  1.0,  -1.0,  0.0,  4.0,  2.0 };
  Matrix<double, 4, 4> mat = data;
  EXPECT_FIT(det(mat), 324.0);
}


TEST(Matrix, Determinant16)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  0.0,  -2.0,  0.0,  1.0,  1.0,  5.0,  4.0,  3.0,  1.0,  7.0,  1.0,  0.0,  0.0,  0.0,  2.0,  0.0,  -3.0,  -1.0,  0.0,  4.0,  2.0,  2.0 };
  Matrix<double, 5, 5> mat = data;
  EXPECT_FIT(det(mat), 996.0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}