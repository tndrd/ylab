#pragma once
#include <cassert>
#include <iostream>

// slow get page imitation
int slow_get_page_int(int key) { return key; }

template<typename CacheT> void count_hits()
{
  int hits = 0;
  int n;
  size_t m;
  
  std::cin >> m >> n;
  assert(std::cin.good());
  CacheT c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup_update(q, slow_get_page_int))
      hits += 1;
  }

  std::cout << hits << std::endl;
}