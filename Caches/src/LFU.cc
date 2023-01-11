#include "LFU.hpp"
#include "hit_count.hpp"

int main()
{
  HWCache::task_hit_count<HWCache::LFU<int>>(std::cin, std::cout);
  return 0;
}