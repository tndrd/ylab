#include "LFU.hpp"
#include "generic_hit_count.hpp"

int main()
{
  count_hits<caches::LFU<int>>();
  return 0;
}