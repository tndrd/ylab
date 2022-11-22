#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "ideal.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

template<typename CacheT> int count_hits(CacheT cache, std::vector<int>& input)
{
  int hits = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (cache.lookup_update(input[i], slow_get_page_int))
      hits += 1;
  }

  return hits;
}

template<> int count_hits(caches::ideal<int> cache, std::vector<int>& input)
{
  int hits = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (cache.lookup_update(input[i], slow_get_page_int, input, i))
      hits += 1;
  }

  return hits;
}

template<typename CacheT> int count_hits_from_stdin()
{
  int n;
  size_t m;
  
  std::cin >> m >> n;
  assert(std::cin.good());

  CacheT cache{m};
  std::vector<int> input;

  for (int i = 0; i < n; ++i)
  {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    input.push_back(q);
  }

  int hits = count_hits(cache, input);

  return hits;
}

template<typename CacheT> int count_hits_from_argv_params(int n_params, char* params[])
{
  assert(n_params > 2);
  assert(n_params - 2 == std::stoi(params[1]));

  CacheT cache{std::stoul(params[0])};
  std::vector<int> input;

  for (int i = 0; i < std::stoi(params[1]); ++i)
  {
    input.push_back(std::stoi(params[2+i]));
  }

  int hits = count_hits(cache, input);

  return hits;
}