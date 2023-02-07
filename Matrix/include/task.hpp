#pragma once

#include "matrix_algorithm.hpp"
#include <cctype>

namespace HWMatrix {
namespace Task {

// Reads matrix of type T from istream
template<typename T>
Matrix<T> read_matrix(std::istream& input)
{
  int dim_input;
  input >> dim_input;
  if (!input.good()) throw std::invalid_argument("Read error");

  if (dim_input <= 0)
    throw std::invalid_argument("Can't create matrix with incorrect dimensions");

  size_t dim  = static_cast<size_t>(dim_input);
  size_t size = dim * dim;
  std::vector<T> data;

  for(int i = 0; i < size; ++i)
  {
    char sep = input.get();
    if (!std::isspace(sep))
      throw std::invalid_argument("Format error");
    
    T val;
    input >> val;
    if (!input.good()) throw std::invalid_argument("Read error");
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