import pairgen
import scramble
from tqdm import tqdm
import numpy as np

N_SCRAMBLES = 3

X_STEP = (pairgen.RBORDER - pairgen.LBORDER) * 2
Y_STEP = (pairgen.TBORDER - pairgen.BBORDER) * 2
Z_STEP = (pairgen.ZBORDER_T - pairgen.ZBORDER_B) * 2

TESTFILE = "triangles.test"

def dump_triangle(tests, tr):
  for i in range(len(tr)):
    for k in range(len(tr[i])):
      tests = tests + f'{tr[i][k]} '
  return tests

def dump_test(tests, intersection, tr1, tr2):
  tests = dump_triangle(tests, tr1)
  tests = dump_triangle(tests, tr2)
  tests = tests + f'{int(intersection)}\n'
  return tests

def pair():
  intersection, tr1, tr2 = pairgen.randpair3D()
  
  for i in range(N_SCRAMBLES):
    tr1, tr2 = scramble.randscramble(tr1, tr2)
  
  return intersection, tr1, tr2

def offset_triangle(tr, vec):
  for i in range(len(tr)):
    tr[i] = tr[i] + vec
  return tr

def offset(tr1, tr2, x, y, z):
  vec = np.array([x * X_STEP, y * Y_STEP, z * Z_STEP])
  tr1 = offset_triangle(tr1, vec)
  tr2 = offset_triangle(tr2, vec)

  return tr1, tr2

def generate_tests(x_n, y_n, z_n):
  test_n = 0
  tests = ""
  for z in tqdm(range(-z_n, z_n)):
    for y in tqdm(range(-y_n, y_n), leave=False):
      for x in range(-x_n, x_n):
        intersection, tr1, tr2 = pair()
        tr1, tr2 = offset(tr1, tr2, x, y, z)
        tests = dump_test(tests, intersection, tr1, tr2)
        test_n = test_n + 1

  return test_n, tests

if __name__ == "__main__":
  with open(TESTFILE, "w") as f:
    x_n, y_n, z_n = map(int, input("Enter dimensions: ").split())
    test_n, tests = generate_tests(x_n, y_n, z_n)
    f.write(f'{test_n}\n' + tests)
  print(f"Successfuly generated {test_n} triangle pairs")
