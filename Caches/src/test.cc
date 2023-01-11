#include <gtest/gtest.h>
#include "test.hpp"
#include "LFU.hpp"
#include "LRU.hpp"
#include "ideal.hpp" 

using namespace HWCache;

TEST(LRU, E2E)
{
  test_cache<LRU<int>>("tests/LRU.test");
}

TEST(LFU, E2E)
{
  test_cache<LRU<int>>("tests/LFU.test");
}

TEST(ideal, E2E)
{
  test_cache<LRU<int>>("tests/ideal.test");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}