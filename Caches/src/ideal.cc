#include "ideal.hpp"
#include <iostream>
#include <cassert>
#include "generic_hit_count.hpp"

int main()
{
  int hits = 0;
  int n;
  size_t m;
  
  std::cin >> m >> n;
  assert(std::cin.good());
  caches::ideal<int> c{m};

  std::vector<int> stream;

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    stream.push_back(q);
    assert(std::cin.good());
  }

  for (int i = 0; i < n; ++i) {
    
    if (c.lookup_update(stream[i], slow_get_page_int, stream, i))
      hits += 1;
  }

  std::cout << hits << std::endl;
}