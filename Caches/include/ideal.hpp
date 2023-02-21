#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <cassert>
#include <optional>
#include <cmath>
#include <iostream>
#include "inputds.hpp"

namespace HWCache
{
template <typename PageT, typename KeyT=int> struct ideal
{
  size_t sz_;
  
  struct IdealUnit
  {
    PageT  page;
    size_t next_pos;
  };

  std::list<IdealUnit> cache_;

  using ListIt      = typename std::list<IdealUnit>::iterator;
  using ConstListIt = typename std::list<IdealUnit>::const_iterator;
  std::unordered_map<KeyT, ListIt>  hash_;

  ideal(size_t sz) : sz_(sz) { }

  void dump() const
  {
    std::cout << "{ ";
    for(ConstListIt it = cache_.cbegin(); it != cache_.cend(); ++it)
    {
      std::cout << it->page << " ";
    }
    std::cout << "}" << std::endl;
  }

  bool full() const { return (cache_.size() == sz_); }

  void add(KeyT key, PageT page, size_t next_pos)
  {
    cache_.push_front({page, next_pos});
    hash_[key] = cache_.begin();
  }

  void remove(ConstListIt it)
  {
    hash_.erase(it->page);
    cache_.erase(it);
  }

  struct CacheFarthest
  {
    size_t pos;
    ConstListIt it;  
  };

  template<typename F>
  void full_cache_update(KeyT key, F slow_get_page, const typename InputDataStorage<KeyT>::Accessor& accessor)
  {
    CacheFarthest cache_farthest = find_farthest_in_cache(accessor);
    size_t input_farthest = accessor.next_pos(key);

    if (input_farthest < cache_farthest.pos)
    {
      remove(cache_farthest.it);
      add(key, slow_get_page(key), input_farthest);
    }
  }

  CacheFarthest find_farthest_in_cache(const typename InputDataStorage<KeyT>::Accessor& accessor) const
  {
    CacheFarthest farthest = {0, cache_.end()}; 

    for (auto it = cache_.begin(); it != cache_.end(); ++it)
    {
      size_t first_occurence = it->next_pos;
      if (first_occurence > farthest.pos)
      {
        farthest.pos = first_occurence;
        farthest.it  = it;
      }
    }
    return farthest;
  }

  template<typename F> bool lookup_update(KeyT key, F slow_get_page, const typename InputDataStorage<KeyT>::Accessor& accessor)
  { 
    if(sz_ == 0) return false;
    
    auto hit = hash_.find(key);

    if (hit == hash_.end())
    {
      if(full())
      {
        full_cache_update(key, slow_get_page, accessor);
      }
      else
      {
        add(key, slow_get_page(key), accessor.next_pos(key));
      }
      return false;
    }

    (hit->second)->next_pos = accessor.next_pos(key);
    return true;
  }
};

} // namespace caches
