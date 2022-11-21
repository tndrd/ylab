#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <iterator>

namespace caches
{

  template<typename PageT, typename KeyT, typename CountT=size_t>
  struct LFU_unit
  {
    PageT  content_; // Page content
    KeyT   key_;     // Corresponding unordered_map key
    CountT count_;   // Frequency counter  
  };

  template <typename PageT, typename KeyT=int> struct LFU
  {
    size_t sz_;
    
    using LFU_unitT = typename caches::LFU_unit<PageT, KeyT, size_t>;
    std::list<LFU_unitT> cache_;
    
    using ListIt      = typename std::list<LFU_unitT>::iterator;
    using ConstListIt = typename std::list<LFU_unitT>::const_iterator;

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

        LFU_unitT new_unit = { slow_get_page(key), key, 1 };
        
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
