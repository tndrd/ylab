#include "matrix.hpp"

using namespace HWMatrix; 

int main()
{
  std::vector<double> values {1.5, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  Matrix<double> mat {5, 2, values};
  std::cout << mat;
}