import numpy as np

def argmax(li):
  maxind = np.argmax(li)
  return maxind, li[maxind] 

def first_occurence(item, li):
    inds = np.where(item == li)[0]
    if len(inds) == 0: return len(li)
    return inds[0]

def find_in_future(item, future, now):
  return first_occurence(item, future[now + 1:])

class IdealCache:
  def __init__(self, size):
    self.size = size
    self.data = np.array([None]*size)
    self.used = 0

  def full(self): return self.used == self.size

  def lookup_update(self, item, future, now):
    
    if self.size == 0: return 0
    
    elif item in self.data:
      return 1
    
    elif not self.full():
      self.data[self.used] = item
      self.used = self.used + 1
      return 0
    
    elif now != len(future) - 1:
      cache_far_ind, cache_far_pos = argmax([find_in_future(item, future, now) for item in self.data])
      input_far_pos = find_in_future(item, future, now)

      if cache_far_pos > input_far_pos:
        self.data[cache_far_ind] = item
    
    return 0

class LRUCache:
  def __init__(self, size):
    self.size = size
    self.data = []

  def full(self): return len(self.data) == self.size

  def lookup_update(self, item, _, __):
    
    if self.size == 0: return 0
    
    elif item in self.data:
      ind = self.data.index(item)
      self.data.pop(ind)
      self.data.append(item)
      return 1
    
    elif not self.full():
      self.data.append(item)
      return 0
    
    else:
      self.data.pop(0)
      self.data.append(item)
    
    return 0


class LFUunit:
  def __init__(self, data):
    self.data = data
    self.f = 0

class LFUCache:
  def __init__(self, size):
    self.size = size
    self.data = []

  def find_lfu(self):
    lfu_ind = 0
    for i in range(1, len(self.data)):
      if self.data[i].f < self.data[lfu_ind].f:
        lfu_ind = i
    return lfu_ind

  def full(self): return len(self.data) == self.size

  def search(self, item):
    ind = None
    for i in range(len(self.data)):
      if self.data[i].data == item:
        return i

  def lookup_update(self, item, _, __):
    
    if self.size == 0: return 0
    
    hit = self.search(item) 
    if hit is not None:
      self.data[hit].f = self.data[hit].f + 1
      return 1
    
    elif not self.full():
      self.data.insert(0, LFUunit(item))
      return 0
    
    else:
      self.data.pop(self.find_lfu())
      self.data.insert(0, LFUunit(item))
    
    return 0

CACHES = {"ideal": IdealCache, "LRU": LRUCache, "LFU": LFUCache}