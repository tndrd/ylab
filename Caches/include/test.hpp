#pragma once
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "hit_count.hpp"

namespace HWCache
{

template<typename CacheT> void test_cache(const char* testfile)
{
  std::ifstream input(testfile);
  ASSERT_TRUE(input.good());

  size_t n_tests;
  input >> n_tests;

  std::stringstream output;

  for (int i = 0; i < n_tests; ++i)
  {
    task_hit_count<CacheT>(input, output);
    
    size_t expected;
    input >> expected;

    size_t got = std::stoul(output.str());
    output.clear();
    output.str("");

    EXPECT_EQ(got, expected);
  }
}
}