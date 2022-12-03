#include "matrix.hpp"

using namespace HWMatrix; 


Matrix<double> read_matrix()
{
  size_t dim;
  std::cin >> dim;
  assert(std::cin.good());

  size_t size = dim * dim;
  std::vector<double> data;

  double val;
  for(int i = 0; i < size; ++i)
  {
    std::cin >> val;
    assert(std::cin.good());
    data.push_back(val);
  }

  return {dim, dim, data};
}

int main()
{
  Matrix<double> mat = read_matrix();
  std::cout << det(mat) << std::endl;
  return 0;
}