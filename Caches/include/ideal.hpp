#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <cassert>

namespace HWCache
{

  template <typename PageT, typename KeyT=int> struct ideal
  {
    size_t sz_;
    std::list<PageT> cache_;

    using ListIt      = typename std::list<PageT>::iterator;
    using ConstListIt = typename std::list<PageT>::const_iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    ideal(size_t sz) : sz_(sz) {}

    bool full() const { return (cache_.size() == sz_); }

    void add(KeyT key, PageT page)
    {
      cache_.push_front(page);
      hash_[key] = cache_.begin();
    }

    void remove(ConstListIt it)
    {
      hash_.erase(*it);
      cache_.erase(it);
    }

    size_t find_first_occurence(KeyT key, std::vector<KeyT>& future, size_t now) const
    {
      for (int i = now; i < future.size(); ++i)
      {
        if (future[i] == key) return i;
      }
      return 0;
    }

    struct CacheFarthest
    {
      size_t pos;
      ConstListIt it;  
    };

    // Finds cache's element that is the farthest in the input stream
    CacheFarthest find_farthest_in_cache(std::vector<KeyT>& future, size_t now) const
    {
      assert(now < future.size());

      CacheFarthest farthest = {0, cache_.end()}; 

      for (auto it = cache_.begin(); it != cache_.end(); ++it)
      {
        size_t first_occurence = find_first_occurence(*it, future, now);
        if (first_occurence > farthest.pos)
        {
          farthest.pos = first_occurence;
          farthest.it  = it;
        }
      }
      
      return farthest;
    }

    template<typename F>
    void full_cache_update(std::vector<KeyT>& future, size_t now, F slow_get_page)
    {
      assert(now < future.size());

      CacheFarthest cache_farthest = find_farthest_in_cache(future, now);
      size_t input_farthest = find_first_occurence(future[now], future, now + 1);
    
      if (cache_farthest.it != cache_.end())
      {
        if (input_farthest != 0 && cache_farthest.pos > input_farthest)
        {
          remove(cache_farthest.it);
          KeyT key = future[now];
          add(key, slow_get_page(key));
        }
      }
      else
      {
        if (input_farthest != 0)
        {
          remove(std::prev(cache_.end()));
          KeyT key = future[now];
          add(key, slow_get_page(key));
        }
      }
    }

    template<typename F> bool lookup_update(KeyT key, F slow_get_page, std::vector<PageT>& future, size_t now)
    {
      assert(now < future.size());
      
      auto hit = hash_.find(key);

      if (hit == hash_.end())
      {
        if(full())
        {
          full_cache_update(future, now, slow_get_page);
        }
        else
        {
          add(key, slow_get_page(key));
        }
        return false;
      }
      
      return true;
    }
  };
} // namespace caches
