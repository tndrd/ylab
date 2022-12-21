#pragma once

#include "matrix.hpp"

namespace HWMatrix {
namespace Task {

template<typename T>
Matrix<T> read_matrix(std::istream& input)
{
  size_t dim;
  input >> dim;
  assert(input.good());

  size_t size = dim * dim;
  std::vector<T> data;

  T val;
  for(int i = 0; i < size; ++i)
  {
    input >> val;
    assert(input.good());
    data.push_back(val);
  }

  return {dim, dim, data};
}

template<typename T>
void task(std::istream& input, std::ostream& output)
{
  Matrix<T> mat = read_matrix<T>(input);
  output << det(mat) << std::endl;
}

}
}