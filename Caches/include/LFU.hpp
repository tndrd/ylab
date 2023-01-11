#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iterator>

namespace HWCache
{
  template <typename PageT, typename KeyT=int> struct LFU
  {

    struct LFU_unit
    {
      PageT  content_; // Page content
      KeyT   key_;     // Corresponding unordered_map key
      size_t count_;   // Frequency counter  
    };

    size_t sz_;
    
    std::list<LFU_unit> cache_;
    
    using ListIt      = typename std::list<LFU_unit>::iterator;
    using ConstListIt = typename std::list<LFU_unit>::const_iterator;

    std::unordered_map<KeyT, ListIt> hash_;


    LFU(size_t sz) : sz_(sz) {}

    bool full() const { return (cache_.size() == sz_); }


    // Finds least frequent element in cache [O(n)]
    // May be optimized
    ConstListIt find_least_frequent() const
    {
      auto least_frequent = cache_.begin();

      for (auto eltit = cache_.begin(); eltit != cache_.end(); eltit++)
      {
        if (eltit->count_ < least_frequent->count_) least_frequent = eltit;
      }

      return least_frequent;
    }


    template<typename F> bool lookup_update(KeyT key, F slow_get_page)
    {
      auto hit = hash_.find(key);

      if (hit == hash_.end())
      {
        if(full())
        {
          auto least_frequent = find_least_frequent(); 
          hash_.erase(least_frequent->key_);
          cache_.erase(least_frequent);
        }

        LFU_unit new_unit = { slow_get_page(key), key, 1 };
        
        cache_.push_front(new_unit);
        hash_[key] = cache_.begin();
        return false;
      }

      auto eltit = hit->second;
      eltit->count_++;
      
      return true;
    }
  };
} // namespace caches
