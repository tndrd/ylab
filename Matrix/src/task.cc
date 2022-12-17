#include "task.hpp"

namespace HWMatrix
{

Matrix<double> read_matrix(std::istream& input)
{
  size_t dim;
  input >> dim;
  assert(input.good());

  size_t size = dim * dim;
  std::vector<double> data;

  double val;
  for(int i = 0; i < size; ++i)
  {
    input >> val;
    assert(input.good());
    data.push_back(val);
  }

  return {dim, dim, data};
}

void task(std::istream& input, std::ostream& output)
{
  Matrix<double> mat = read_matrix(input);
  output << det(mat) << std::endl;
}

}