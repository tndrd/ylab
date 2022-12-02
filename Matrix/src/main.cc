#include "matrix.hpp"

using namespace HWMatrix;

int main()
{
  std::vector<int> values {1, 2, 3, 4, 5, 6, 7, 8, 9};
  //Matrix<int, 3, 3> mat {values};

  //std::cout << "--------" << std::endl;

  //int a = mat[0][1];

  auto a = Matrix<int, 3, 3>{values}.swap_rows(0, 1).swap_rows(0, 1);

  std::cout << a << std::endl;
}