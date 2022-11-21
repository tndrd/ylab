#include "LRU.hpp"
#include "generic_hit_count.hpp"

int main()
{
  count_hits<caches::LRU<int>>();
  return 0;
}