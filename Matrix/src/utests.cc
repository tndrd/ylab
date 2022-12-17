#include <gtest/gtest.h>
#include "task.hpp"

using namespace HWMatrix;

static const double ABS_ERROR = 0.000001; 

TEST(Matrix, Access)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Matrix<double> mat {3, 3, data};

  EXPECT_EQ(mat[2][2], 9);
  mat[2][2] = 42;
  EXPECT_EQ(mat[2][2], 42);
}

TEST(Matrix, ConstAccess)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  const Matrix<double> mat {3, 3, data};

  EXPECT_EQ(mat[2][2], 9);
  //mat[2][2] = 42;      // Doesn't compile, great! 
  //mat.swap_rows(1, 2); // 
}

TEST(Matrix, RvalueAccess)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};

  double a = Matrix<double>{3, 3, data}.swap_rows(0,1).swap_rows(1,2)[2][2];
  EXPECT_EQ(a, 3);
}

TEST(Matrix, CopyCtorSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Matrix<double> mat {3, 3, data};
  Matrix<double> copy1 = mat;
  
  EXPECT_EQ(copy1[0][0], 1);
  EXPECT_EQ(mat[0][0], 1);
  copy1.swap_rows(0, 1);
  EXPECT_EQ(copy1[0][0], 4);
  EXPECT_EQ(mat[0][0], 1);

  mat.swap_rows(0,2);
  EXPECT_EQ(mat[0][0], 7);
  Matrix<double> copy2 = mat;
  EXPECT_EQ(copy2[0][0], 7);
}

TEST(Matrix, CopyAssignDifferentDims)
{
  std::vector<double> data1 {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> data2 {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  Matrix<double> mat_old {3, 3, data1};
  Matrix<double> mat_new {5, 2, data2};

  mat_old.swap_rows(0,1).swap_rows(1,2);
  mat_new.swap_rows(0,4).swap_rows(0,2);

  mat_old = mat_new;
  
  EXPECT_EQ(mat_old[2][1], 1);
  EXPECT_EQ(mat_old[4][1], 0);

  EXPECT_EQ(mat_new[2][1], 1);
  EXPECT_EQ(mat_new[4][1], 0);
}

TEST(Matrix, CopyAssignSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> empt {0, 0, 0, 0, 0, 0, 0, 0, 0};
  Matrix<double> mat {3, 3, data};
  Matrix<double> copy1 {3, 3, empt};
  copy1.swap_rows(0,1);
  copy1 = mat;
  
  
  EXPECT_EQ(copy1[0][0], 1);
  EXPECT_EQ(mat[0][0], 1);
  copy1.swap_rows(0, 1);
  EXPECT_EQ(copy1[0][0], 4);
  EXPECT_EQ(mat[0][0], 1);

  mat.swap_rows(0,2);
  EXPECT_EQ(mat[0][0], 7);
  Matrix<double> copy2 {3, 3, empt};
  copy2.swap_rows(0,1);
  copy2 = mat;
  EXPECT_EQ(copy2[0][0], 7);
}

TEST(Matrix, MoveCtorSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  
  Matrix<double> newm = Matrix<double>{3, 3, data}.swap_rows(0,1).swap_rows(1,2);

  EXPECT_EQ(newm[0][0], 4);
  EXPECT_EQ(newm[1][0], 7);
  EXPECT_EQ(newm[2][0], 1);
}

TEST(Matrix, MoveAssignSwap)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<double> empt {0, 0, 0, 0, 0, 0, 0, 0, 0};

  Matrix<double> newm {3, 3, empt};
  newm.swap_rows(0, 1);

  newm = Matrix<double>{3, 3, data}.swap_rows(0,1).swap_rows(1,2);

  EXPECT_EQ(newm[0][0], 4);
  EXPECT_EQ(newm[1][0], 7);
  EXPECT_EQ(newm[2][0], 1);
}

TEST(Matrix, ConstructExceptions)
{
  EXPECT_THROW(({
    Matrix<double> newm {0, 1};
  }), std::invalid_argument);

  EXPECT_THROW(({
    Matrix<double> newm {1, 0};
  }), std::invalid_argument);

  EXPECT_THROW(({
    Matrix<double> newm {0, 0};
  }), std::invalid_argument);  
}

TEST(Matrix, VectorConstructExceptions)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  EXPECT_NO_THROW(({
    Matrix<double> newm {1, 12, data};
  }));

  EXPECT_NO_THROW(({
    Matrix<double> newm {12, 1, data};
  }));

  EXPECT_NO_THROW(({
    Matrix<double> newm {6, 2, data};
  }));

  EXPECT_NO_THROW(({
    Matrix<double> newm {2, 6, data};
  }));

  EXPECT_NO_THROW(({
    Matrix<double> newm {3, 4, data};
  }));

  EXPECT_NO_THROW(({
    Matrix<double> newm {4, 3, data};
  }));

  EXPECT_THROW(({
    Matrix<double> newm {1, 1, data};
  }), std::invalid_argument);

  EXPECT_THROW(({
    Matrix<double> newm {5, 2, data};
  }), std::invalid_argument);

  EXPECT_THROW(({
    Matrix<double> newm {42, 13, data};
  }), std::invalid_argument);  
}

TEST(Matrix, AccessExceptions)
{
  Matrix<double> test {3, 4};

  EXPECT_THROW(({
    test[3][2];
  }), std::out_of_range);

  EXPECT_THROW(({
    test[2][4];
  }), std::out_of_range);

  EXPECT_THROW(({
    test[0][42];
  }), std::out_of_range);

  EXPECT_THROW(({
    test[42][0];
  }), std::out_of_range);

  EXPECT_NO_THROW(({
    test[0][0];
  }));

  EXPECT_NO_THROW(({
    test[2][3];
  }));
}

TEST(Matrix, DeterminantExceptions)
{
  Matrix<double> test {3, 4};

  EXPECT_THROW(({
    det(test);
  }), std::invalid_argument);
}

// GENERATED TESTS

TEST(Matrix, Determinant1)
{
  std::vector<double> data  { 42.0 };
  Matrix<double> mat (1, 1, data);
  EXPECT_NEAR(det(mat), 42.0, ABS_ERROR);
}


TEST(Matrix, Determinant2)
{
  std::vector<double> data  { -42.0 };
  Matrix<double> mat (1, 1, data);
  EXPECT_NEAR(det(mat), -42.0, ABS_ERROR);
}


TEST(Matrix, Determinant3)
{
  std::vector<double> data  { 0.0 };
  Matrix<double> mat (1, 1, data);
  EXPECT_NEAR(det(mat), 0.0, ABS_ERROR);
}


TEST(Matrix, Determinant4)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  1.0 };
  Matrix<double> mat (2, 2, data);
  EXPECT_NEAR(det(mat), 1.0, ABS_ERROR);
}


TEST(Matrix, Determinant5)
{
  std::vector<double> data  { 0.0,  1.0,  1.0,  0.0 };
  Matrix<double> mat (2, 2, data);
  EXPECT_NEAR(det(mat), -1.0, ABS_ERROR);
}


TEST(Matrix, Determinant6)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0 };
  Matrix<double> mat (2, 2, data);
  EXPECT_NEAR(det(mat), 0.0, ABS_ERROR);
}


TEST(Matrix, Determinant7)
{
  std::vector<double> data  { 11.0,  -2.0,  7.0,  5.0 };
  Matrix<double> mat (2, 2, data);
  EXPECT_NEAR(det(mat), 69.0, ABS_ERROR);
}


TEST(Matrix, Determinant8)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0,  1.0 };
  Matrix<double> mat (3, 3, data);
  EXPECT_NEAR(det(mat), 1.0, ABS_ERROR);
}


TEST(Matrix, Determinant9)
{
  std::vector<double> data  { 0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,  0.0 };
  Matrix<double> mat (3, 3, data);
  EXPECT_NEAR(det(mat), 1.0, ABS_ERROR);
}


TEST(Matrix, Determinant10)
{
  std::vector<double> data  { 3.0,  3.0,  -1.0,  4.0,  1.0,  3.0,  1.0,  -2.0,  -2.0 };
  Matrix<double> mat (3, 3, data);
  EXPECT_NEAR(det(mat), 54.0, ABS_ERROR);
}


TEST(Matrix, Determinant11)
{
  std::vector<double> data  { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0 };
  Matrix<double> mat (3, 3, data);
  EXPECT_NEAR(det(mat), 0.0, ABS_ERROR);
}


TEST(Matrix, Determinant12)
{
  std::vector<double> data  { 1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  21.0,  0.0,  0.0,  0.0,  0.0,  2.0 };
  Matrix<double> mat (4, 4, data);
  EXPECT_NEAR(det(mat), 42.0, ABS_ERROR);
}


TEST(Matrix, Determinant13)
{
  std::vector<double> data  { -2.0,  1.0,  3.0,  2.0,  3.0,  0.0,  -1.0,  2.0,  -5.0,  2.0,  3.0,  0.0,  4.0,  -1.0,  2.0,  -3.0 };
  Matrix<double> mat (4, 4, data);
  EXPECT_NEAR(det(mat), -80.0, ABS_ERROR);
}


TEST(Matrix, Determinant14)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  -2.0,  0.0,  1.0,  5.0,  4.0,  3.0,  1.0,  1.0,  0.0,  -1.0,  0.0,  2.0,  2.0 };
  Matrix<double> mat (4, 4, data);
  EXPECT_NEAR(det(mat), -12.0, ABS_ERROR);
}


TEST(Matrix, Determinant15)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  0.0,  0.0,  1.0,  1.0,  5.0,  3.0,  1.0,  7.0,  1.0,  -1.0,  0.0,  4.0,  2.0 };
  Matrix<double> mat (4, 4, data);
  EXPECT_NEAR(det(mat), 324.0, ABS_ERROR);
}


TEST(Matrix, Determinant16)
{
  std::vector<double> data  { -1.0,  -4.0,  0.0,  0.0,  -2.0,  0.0,  1.0,  1.0,  5.0,  4.0,  3.0,  1.0,  7.0,  1.0,  0.0,  0.0,  0.0,  2.0,  0.0,  -3.0,  -1.0,  0.0,  4.0,  2.0,  2.0 };
  Matrix<double> mat (5, 5, data);
  EXPECT_NEAR(det(mat), 996.0, ABS_ERROR);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}