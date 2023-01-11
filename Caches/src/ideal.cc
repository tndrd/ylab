#include "ideal.hpp"
#include <iostream>
#include <cassert>
#include "hit_count.hpp"

int main()
{
  HWCache::task_hit_count<HWCache::ideal<int>>(std::cin, std::cout);
  return 0;
}