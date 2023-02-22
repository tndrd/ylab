#include "ideal_old.hpp"
#include "hit_count.hpp"

int main()
{
  HWCache::task_hit_count<HWCache::ideal_old<int>>(std::cin, std::cout);
  return 0;
}