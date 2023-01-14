import pairgen
import scramble
from tqdm import tqdm
import numpy as np

N_SCRAMBLES = 10

X_STEP = (pairgen.RBORDER - pairgen.LBORDER) * 20
Y_STEP = (pairgen.TBORDER - pairgen.BBORDER) * 20
Z_STEP = (pairgen.ZBORDER_T - pairgen.ZBORDER_B) * 20

UNIT_TEST_FILE = "triangles.test"
E2E_TEST_FILE  = "e2e.test"

def dump_triangle(tests, tr):
  for i in range(len(tr)):
    for k in range(len(tr[i])):
      tests = tests + f'{tr[i][k]} '
  return tests

def dump_test(tests, tr1, tr2):
  tests = dump_triangle(tests, tr1)
  tests = dump_triangle(tests, tr2)
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
  intersects = 0
  
  unit_tests = ""
  e2e_tests  = ""
  e2e_result = []
  
  for z in tqdm(range(-z_n, z_n)):
    for y in tqdm(range(-y_n, y_n), leave=False):
      for x in range(-x_n, x_n):
        intersection, tr1, tr2 = pair()
        tr1, tr2 = offset(tr1, tr2, x, y, z)
        
        unit_tests = dump_test(unit_tests, tr1, tr2)
        unit_tests = unit_tests + f'{int(intersection)}\n'
        
        e2e_tests = dump_test(e2e_tests, tr1, tr2)
        if intersection:
          e2e_result.append(2*test_n)
          e2e_result.append(2*test_n + 1)
          intersects = intersects + 2

        test_n = test_n + 1

  e2e_result.sort()

  e2e_result_string = ""
  for x in e2e_result: 
    e2e_result_string = e2e_result_string + f'{x} ' 
  
  return test_n, intersects, unit_tests, e2e_tests, e2e_result_string

if __name__ == "__main__":
  x_n, y_n, z_n = map(int, input("Enter dimensions: ").split())
  test_n, intersects, unit_tests, e2e_tests, e2e_result = generate_tests(x_n, y_n, z_n)
  
  with open(UNIT_TEST_FILE, "w") as f:
    f.write(f'{test_n}\n' + unit_tests)

  with open(E2E_TEST_FILE, "w") as f:
    f.write(f'{test_n * 2}\n' + e2e_tests + f'\n{intersects}\n' + e2e_result)

  print(f"Successfuly generated {test_n} triangle pairs")
  print(f"Number of intersections: {intersects}")
