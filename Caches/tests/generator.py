from prototypes import *
import random
from tqdm import tqdm
import numpy as np

import json

class GenConfig:
  def __init__(self, name, cache_t, cache_size, data_len, maxval, amount=1):
    self.MIN_CACHE_SIZE, self.MAX_CACHE_SIZE = cache_size
    self.MIN_DATA_LEN,   self.MAX_DATA_LEN   = data_len
    self.amount = amount
    self.name = name
    self.maxval = maxval
    self.cache_t = cache_t

def run_prototype(cache_t, cache_size, testdata):
  cache = cache_t(cache_size)
  hits = 0

  for i in tqdm(range(len(testdata)), leave=False):
    hit = cache.lookup_update(testdata[i], testdata, i)
    hits += hit

  return hits

def get_test(cache_t, cache_size, testdata):
  return cache_size, testdata, run_prototype(cache_t, cache_size, testdata)

def test_to_string(cache_size, testdata, result):
  testdata_str = " ".join(map(str, testdata))
  return f'{cache_size} {len(testdata)} {testdata_str} {result}'

def gen_single_test(config):
  cache_size = random.randint(config.MIN_CACHE_SIZE, config.MAX_CACHE_SIZE)
  data_len   = random.randint(config.MIN_DATA_LEN,   config.MAX_DATA_LEN)
  dataset    = set(range(0, config.maxval))
  testdata   = np.array([random.sample(dataset, 1)[0] for i in range(data_len)])

  test = get_test(config.cache_t, cache_size, testdata)
  return test_to_string(*test)

def gen_tests(config):
  tests = f'{config.amount}\n'

  for i in tqdm(range(config.amount)):
    tests = tests + gen_single_test(config) + '\n'

  return tests

def gen_all_tests(config):
  print(f"Generating benchmark {config.name}:")
  tests = gen_tests(config)
  with open("./build/" + config.name + '_gen.test', 'w') as f:
    f.write(tests)

def load_config(benchmark):
  return GenConfig(benchmark["name"], CACHES[benchmark["cache"]], benchmark["cache_size"], benchmark["data_len"], benchmark["maxval"], benchmark["amount"])

def main():
  with open("benchmarks.json") as f:
    d = json.load(f)
    for benchmark in d["benchmarks"]:
      config = load_config(benchmark)
      gen_all_tests(config)

if __name__ == "__main__":
  main()