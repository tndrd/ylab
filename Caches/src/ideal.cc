#include "ideal.hpp"
#include <iostream>
#include <cassert>
#include "hit_count.hpp"

int main()
{
  int hits = count_hits_from_stdin<caches::ideal<int>>();
  std::cout << hits << std::endl;
  return 0;
}