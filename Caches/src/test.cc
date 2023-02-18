#include <gtest/gtest.h>
#include "test.hpp"
#include "LFU.hpp"
#include "LRU.hpp"
#include "ideal.hpp" 

using namespace HWCache;

TEST(LRU, E2E_hardcoded)
{
  test_cache<LRU<int>>("tests/LRU.test");
}

TEST(LRU, E2E_gen)
{
  test_cache<LRU<int>>("tests/LRU_gen.test");
}

TEST(LFU, E2E_hardcoded)
{
  test_cache<LFU<int>>("tests/LFU.test");
}

TEST(LFU, E2E_gen)
{
  test_cache<LFU<int>>("tests/LFU_gen.test");
}

TEST(ideal, E2E_hardcoded)
{
  test_cache<ideal<int>>("tests/ideal.test");
}

TEST(ideal, E2E_gen)
{
  test_cache<ideal<int>>("tests/ideal_gen.test");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}