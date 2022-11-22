#include "LRU.hpp"
#include "hit_count.hpp"

int main()
{
  int hits = count_hits_from_stdin<caches::LRU<int>>();
  std::cout << hits << std::endl;
  return 0;
}