#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "ideal.hpp"
#include "ideal_old.hpp"

namespace HWCache
{

// slow get page imitation
int slow_get_page_int(int key) { return key; }

template<typename CacheT, typename AccessT> size_t count_hits(CacheT& cache, AccessT& accessor)
{
  size_t hits = 0;
  
  while(!accessor.end())
  { 
    int key = accessor.get();
    if (cache.lookup_update(key, slow_get_page_int))
      hits += 1;
  }

  return hits;
}

template<> size_t count_hits(ideal<int>& cache, typename InputDataStorage<int>::Accessor& accessor)
{
  size_t hits = 0;
  
  while(!accessor.end())
  { 
    int key = accessor.get();
    if (cache.lookup_update(key, slow_get_page_int, accessor))
      hits += 1;
  }

  return hits;
}

template<> size_t count_hits(ideal_old<int>& cache, typename InputDataStorage<int>::Accessor& accessor)
{
  size_t hits = 0;
  const std::vector<int>& data = accessor.plain_data();
  for(int i = 0; i < data.size(); ++i)
  { 
    int key = data[i];
    if (cache.lookup_update(key, slow_get_page_int, data, i))
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

  InputDataStorage<int> ids;

  for (int i = 0; i < n; ++i)
  {
    int q;
    input >> q;
    assert(input.good());
    ids.add(q);
  }

  InputDataStorage<int>::Accessor accessor (ids);
  CacheT cache{m};

  output << count_hits(cache, accessor);
}

}