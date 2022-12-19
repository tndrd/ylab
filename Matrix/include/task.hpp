#pragma once

#include "matrix.hpp"

namespace HWMatrix
{
Matrix<double> read_matrix(std::istream& input);
void task(std::istream& input, std::ostream& output);
}