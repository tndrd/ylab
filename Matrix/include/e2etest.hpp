#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "task.hpp"

namespace HWMatrix {
namespace Testing  {

template<typename T>
void e2e_test(const std::string& tests_file, const double tolerance)
{
  std::ifstream input(tests_file);
  ASSERT_TRUE(input.good());
  std::stringstream output;
  
  size_t n_tests;
  input >> n_tests;

  std::cerr << "[          ] [ INFO ]" << "Testing " << n_tests << " determinant calculations" << std::endl;

  for (int i = 0; i < n_tests; ++i)
  {
    task(input, output);
    
    double result = std::stod(output.str());
    output.clear();
    output.str("");

    double expected;
    input >> expected;

    EXPECT_NEAR(expected, result, tolerance);
  }
}

}
}