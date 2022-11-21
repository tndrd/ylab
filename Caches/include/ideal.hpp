#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>

namespace caches
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


    // Finds cache's element that is the farthest in the input stream
    ConstListIt find_farthest(std::vector<KeyT> future, int now) const
    {
      for (int i = future.size() - 1; i <= now; i--)
      {
        auto hit = hash_.find(future[i]);
        if (hit != hash_.end()) return hit->second;
      }

      // if all of the cache content does not exist in the future, return the least recently used one
      // this behavior won't affect the hit count 
      return std::prev(cache_.end());
    }

    template<typename F> bool lookup_update(KeyT key, F slow_get_page, std::vector<PageT> future, int now)
    {
      auto hit = hash_.find(key);

      if (hit == hash_.end())
      {
        if(full())
        {
          auto farthest = find_farthest(future, now);
          hash_.erase(*farthest);
          cache_.erase(farthest);
        }

        cache_.push_front(slow_get_page(key));
        hash_[key] = cache_.begin();
        return false;
      }
      
      return true;
    }
  };
} // namespace caches
