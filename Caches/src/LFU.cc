#include "LFU.hpp"
#include "hit_count.hpp"

int main()
{
  int hits = count_hits_from_stdin<caches::LFU<int>>();
  std::cout << hits << std::endl;
  return 0;
}