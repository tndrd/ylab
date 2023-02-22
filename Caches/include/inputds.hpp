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

namespace HWCache
{

// Data structure that stores sequence of keys.
// Each key is mapped to vector of its locations in sequence
template<typename KeyT>
class InputDataStorage
{
  using DataT  = typename std::vector<KeyT>;
  using DataIt = typename std::vector<KeyT>::const_iterator;

  using HashEntryT  = typename std::vector<size_t>;
  using HashEntryIt = typename HashEntryT::const_iterator;
  
  private:
  DataT data_;
  DataT keys_;
  std::unordered_map<KeyT, HashEntryT> hash_;

  public:
  InputDataStorage() {}

  private:

  // Adds pos to location vectors of key
  void hash_add_pos(KeyT key, size_t pos)
  {
    auto entry = hash_.find(key);

    if(entry == hash_.end())
    {
      hash_[key] = {pos};
      keys_.push_back(key);
    }
    else
    {
      assert(*std::prev((entry->second).end()) < pos);
      entry->second.push_back(pos);
    }
  }

  public:

  // Adds key to sequence and stores its location
  void add(KeyT key)
  {
    size_t pos = data_.size();
    data_.push_back(key);
    hash_add_pos(key, pos);
  }

  const std::unordered_map<KeyT, HashEntryT>& hash() const noexcept
  {
    return hash_;
  }

  const std::vector<KeyT>& data() const noexcept
  {
    return data_;
  }

  const std::vector<KeyT>& keys() const noexcept
  {
    return keys_;
  }

  const DataIt cbegin() const noexcept
  {
    return data_.cbegin();
  }

  const DataIt cend() const noexcept
  {
    return data_.cend();
  }

// Class for reading InputDataStorage
// Reads data and processes keys' locations ->
// -> relevant to current reading position
class Accessor
{
  
  const  InputDataStorage<KeyT>& storage_; // Refers to IDS without modifying it
  size_t now_; // Current reading position

  // Class that grants serial access
  // to list of mapped key positions in input sequence
  class HashEntryAccessor
  {
    private:
    const HashEntryIt end_; // End of position list
    HashEntryIt it_;        // Iterator to next key occurence in input sequence

    public:

    // Refers to list of mapped key's positions
    explicit HashEntryAccessor(const HashEntryT& entry): end_(entry.cend()), it_(entry.cbegin()) { }

    struct SearchResult
    {
      bool found;
      size_t pos = 0;
    };

    // Return next key occurence position
    SearchResult next_pos() const noexcept
    {
      if (it_ == end_)
      {
        return {false};
      }
      return {true, *it_};
    }

    // "Steps" the key position in input sequence
    // After use it_ points to subsequent key position 
    void step() noexcept
    {
      assert(it_ != end_);
      it_++;
    }
  };

  // Map Keys -> Accessors to positions
  std::unordered_map<KeyT, HashEntryAccessor> loc_;

  // Initialization of map
  void fill_loc_map()
  {
    const std::vector<KeyT>& keys = storage_.keys();
    const std::unordered_map<KeyT, HashEntryT>& hash = storage_.hash();
    
    for (int i = 0; i < keys.size(); ++i)
    {
      KeyT key = keys[i];
      assert(hash.find(key) != hash.end());
      loc_.emplace(key, HashEntryAccessor(hash.find(key) -> second));
    }
  }

  public:

  // Just a debug dump
  void dump() const noexcept
  {
    auto keys = storage_.keys();
    auto hash = storage_.hash();

    for (int i = 0; i < keys.size(); ++i)
    {
      KeyT key = keys[i];
      std::cout << "[" << key << "]" << "{ ";
      assert(loc_.find(key) != loc_.end());

      auto hit = loc_.find(key) -> second;
      
      for (HashEntryIt it = hit.it_; it != hit.end_; ++it)
      {
        std::cout << *it << " ";
      }

      std::cout << "}" << std::endl;
    }
  }

  // All the input data is readen
  bool end() const noexcept {return now_ == storage_.data().size();}

  explicit Accessor(const InputDataStorage<KeyT>& storage) noexcept : storage_(storage), now_(0)
  {
    fill_loc_map();
  }

  // Reads the next value and "steps" its position
  KeyT get() noexcept
  {
    assert(!end());
    KeyT key = storage_.data()[now_++];
    (loc_.find(key)->second).step();
    return key;
  }

  // Returns the next key occurence position [O(1)]
  size_t next_pos(KeyT key) const
  {
    assert(loc_.find(key) != loc_.end());
    auto pos = (loc_.find(key)->second).next_pos();
    return (pos.found) ? pos.pos : storage_.data().size();
  }

  // Compatibility :)
  const std::vector<KeyT>& plain_data() const noexcept
  {
    return storage_.data();
  }
};

};

} // namespace caches
