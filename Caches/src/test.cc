#include <gtest/gtest.h>
#include "test.hpp"
#include "LFU.hpp"
#include "LRU.hpp"
#include "ideal.hpp" 

using namespace HWCache;

// -------------- GENERAL -----------------------------

TEST(LRU, E2E_hardcoded)
{
  test_cache<LRU<int>>("tests/build/LRU.test");
}

TEST(LRU, E2E_gen)
{
  test_cache<LRU<int>>("tests/build/LRU_gen.test");
}

TEST(LFU, E2E_hardcoded)
{
  test_cache<LFU<int>>("tests/build/LFU.test");
}

TEST(LFU, E2E_gen)
{
  test_cache<LFU<int>>("tests/build/LFU_gen.test");
}

TEST(ideal, E2E_hardcoded)
{
  test_cache<ideal<int>>("tests/build/ideal.test");
}

TEST(ideal, E2E_gen)
{
  test_cache<ideal<int>>("tests/build/ideal_gen.test");
}

// -------------- IDEAL PERFORMANCE ----------------------

TEST(ideal, E2E_perf)
{
  test_cache<ideal<int>>("tests/build/ideal_perf.test");
}

TEST(ideal_old, E2E_perf)
{
  test_cache<ideal_old<int>>("tests/build/ideal_perf.test");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}