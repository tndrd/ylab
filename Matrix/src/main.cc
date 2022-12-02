#include "matrix.hpp"

using namespace HWMatrix; 

int main()
{
  std::vector<double> values {1.5, 0, 0, 0, 0, 0, 0, 0, 0};
  Matrix<double, 3, 3> mat {values};
  std::cout << mat;
}