import pairgen
import scramble
import numpy as np

N_SCRAMBLES = 3

X_STEP = (pairgen.RBORDER - pairgen.LBORDER) * 2
Y_STEP = (pairgen.TBORDER - pairgen.BBORDER) * 2
Z_STEP = (pairgen.ZBORDER_T - pairgen.ZBORDER_B) * 2

TESTFILE = "triangles.test"

def dump_triangle(f, tr):
  for i in range(len(tr)):
    for k in range(len(tr[i])):
      f.write(f'{tr[i][k]} ')

def dump_test(f, test_n, intersection, tr1, tr2):
  f.write(f'{test_n} ')
  dump_triangle(f, tr1)
  dump_triangle(f, tr2)
  f.write(f'{int(intersection)}\n')

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

def generate_tests(f, x_n, y_n, z_n):
  test_n = 1
  
  for z in range(-z_n, z_n):
    for y in range(-y_n, y_n):
      for x in range(-x_n, x_n):
        intersection, tr1, tr2 = pair()
        tr1, tr2 = offset(tr1, tr2, x, y, z)
        dump_test(f, test_n, intersection, tr1, tr2)
        test_n = test_n + 1

  return test_n

if __name__ == "__main__":
  with open(TESTFILE, "w") as f:
    x_n, y_n, z_n = map(int, input().split())
    test_n = generate_tests(f, x_n, y_n, z_n)
  print(f"Successfuly generated {test_n} triangle pairs")
