#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "ideal.hpp"

namespace HWCache
{

// slow get page imitation
int slow_get_page_int(int key) { return key; }

template<typename CacheT> size_t count_hits(CacheT cache, std::vector<int>& input)
{
  size_t hits = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (cache.lookup_update(input[i], slow_get_page_int))
      hits += 1;
  }

  return hits;
}

template<> size_t count_hits(ideal<int> cache, std::vector<int>& input)
{
  size_t hits = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (cache.lookup_update(input[i], slow_get_page_int, input, i))
      hits += 1;
  }

  return hits;
}

template<typename CacheT> void task_hit_count(std::istream& input, std::ostream& output)
{
  size_t n;
  size_t m;
  
  input >> m >> n;
  assert(input.good());

  CacheT cache{m};
  std::vector<int> input_data;

  for (int i = 0; i < n; ++i)
  {
    int q;
    input >> q;
    assert(input.good());
    input_data.push_back(q);
  }

  output << count_hits(cache, input_data);
}

}