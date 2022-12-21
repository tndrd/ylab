#include "e2etest.hpp"

using namespace HWMatrix;

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

TEST(Matrix, CopyAnotherType)
{
  std::vector<double> data {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Matrix<double> mat {5, 2, data};
  mat.swap_rows(0,1);
  mat.swap_rows(1, 2);
  mat[0][0] = 27;
  mat[0][1] = 42.42;

  Matrix<int> newm {3, 3};
  newm.copy(mat);
  
  EXPECT_EQ(newm[0][0], 27);
  EXPECT_EQ(newm[0][0] / 2, 13);
  EXPECT_EQ(newm[0][1], 42);
  EXPECT_EQ(newm[2][0], 1);
  EXPECT_EQ(newm[2][1], 2);
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


static const char*  TESTS_FILE = "tests/build.test"; 

// Determinant tests with standard precision (EvalT = double) 
static const double DOUBLE_DETERMINANT_TOLERANCE = 0.001;

TEST(Matrix, DeterminantDouble)
{
  Testing::e2e_test<double, double>(TESTS_FILE, DOUBLE_DETERMINANT_TOLERANCE);
}

TEST(Matrix, DeterminantFloat)
{
  Testing::e2e_test<float, double>(TESTS_FILE, DOUBLE_DETERMINANT_TOLERANCE);
}

TEST(Matrix, DeterminantInt)
{
  Testing::e2e_test<int, double>(TESTS_FILE, DOUBLE_DETERMINANT_TOLERANCE);
}

// Determinant tests with high precision (EvalT = long double)
static const double LONG_DOUBLE_DETERMINANT_TOLERANCE = 0.00000001;

TEST(Matrix, DeterminantDoubleHighPrecision)
{
  Testing::e2e_test<double, long double>(TESTS_FILE, LONG_DOUBLE_DETERMINANT_TOLERANCE);
}

TEST(Matrix, DeterminantFloatHighPrecision)
{
  Testing::e2e_test<float, long double>(TESTS_FILE, LONG_DOUBLE_DETERMINANT_TOLERANCE);
}

TEST(Matrix, DeterminantIntHighPrecision)
{
  Testing::e2e_test<int, long double>(TESTS_FILE, LONG_DOUBLE_DETERMINANT_TOLERANCE);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}