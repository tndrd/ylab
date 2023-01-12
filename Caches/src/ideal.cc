#include "ideal.hpp"
#include "hit_count.hpp"

int main()
{
  HWCache::task_hit_count<HWCache::ideal<int>>(std::cin, std::cout);
  return 0;
}