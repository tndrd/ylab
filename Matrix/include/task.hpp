#pragma once

#include "matrix_algorithm.hpp"
#include <cassert>

namespace HWMatrix {
namespace Task {

// Reads matrix of type T from istream
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

// Executes calculation of determinant with subsequent output
template<typename T, typename EvalT=double>
void task(std::istream& input, std::ostream& output)
{
  try
  {
    Matrix<T> mat = read_matrix<T>(input);
    output << det<T, EvalT>(mat) << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cerr << "Failed to process matrix: " << e.what() << '\n';
  }
}

}
}