#include "matrix.hpp"

using namespace HWMatrix; 

int main()
{
  std::vector<double> values {1.5, 0, 0, 0, 0, 0, 0, 0, 0};
  auto mat = Matrix<double, 3, 3>{values}.swap_rows(1,2);

  //mat.gugugaga();

  std::cout << mat;
}