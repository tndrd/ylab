from prototypes import *
import random
from tqdm import tqdm

MIN_CACHE_SIZE = 0
MAX_CACHE_SIZE = 20

MIN_DATA_LEN = 0
MAX_DATA_LEN = 1000

DATASET = set(range(0, 50))

def run_prototype(cache_t, cache_size, testdata):
  cache = cache_t(cache_size)
  hits = 0

  for i in range(len(testdata)):
    hit = cache.lookup_update(testdata[i], testdata, i)
    hits += hit

  return hits

def get_test(cache_t, cache_size, testdata):
  return cache_size, testdata, run_prototype(cache_t, cache_size, testdata)

def test_to_string(cache_size, testdata, result):
  testdata_str = " ".join(map(str, testdata))
  return f'{cache_size} {len(testdata)} {testdata_str} {result}'

def gen_single_test(cache_t):
  cache_size = random.randint(MIN_CACHE_SIZE, MAX_CACHE_SIZE)
  data_len   = random.randint(MIN_DATA_LEN, MAX_DATA_LEN)
  testdata   = [random.sample(DATASET, 1)[0] for i in range(data_len)]

  test = get_test(cache_t, cache_size, testdata)
  return test_to_string(*test)

def gen_tests(cache_t, amount):
  tests = f'{amount}\n'

  for i in tqdm(range(amount)):
    tests = tests + gen_single_test(cache_t) + '\n'

  return tests

def gen_all_tests(amount):
  for cache_t, name in CACHES:
    tests = gen_tests(cache_t, amount)
    with open(name + '_gen.test', 'w') as f:
      f.write(tests)

def main():
  amount = int(input("Enter amount of tests: "))
  gen_all_tests(amount)

if __name__ == "__main__":
  main()
  