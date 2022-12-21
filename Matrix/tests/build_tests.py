from random import randint

MAXSCRAMBLES = 5
MAXCOEFF = 2


def gen_test(mat, det):
  test = f"{len(mat)} "

  for i in mat:
    for k in i:
      test += f"{int(k)} "

  test += str(det)

  return test

def gen_hardcoded_test(dim, val, res):
  test = f"{dim} "
  
  for v in val:
    test += f"{int(v)} "
  
  test += str(res)
  
  return test


def create_identity_matrix(size):
  size = int(size)
  mat = [[0 for col in range(size)] for row in range(size)]

  for i in range(size):
    mat[i][i] = 1

  return mat

def swap_row(mat, r1, r2):
  if (r1 == r2): raise NameError("Can't swap row with itself")
  mat[r1], mat[r2] = mat[r2], mat[r1] 
  
def swap_column(mat, c1, c2):
  if (c1 == c2): raise NameError("Can't swap column with itself")
  for i in range(len(mat[0])):
    mat[i][c1], mat[i][c2] = mat[i][c2], mat[i][c1]


def add_row(mat, coeff, r1, r2):
  if (r1 == r2): raise NameError("Can't add row with itself")
  for i in range(len(mat[0])):
    mat[r1][i] += coeff * mat[r2][i]


def add_column(mat, coeff, c1, c2):
  if (c1 == c2): raise NameError("Can't add column with itself")
  for i in range(len(mat[0])):
    mat[i][c1] += coeff * mat[i][c2]


def rdind(mat):
  return randint(0, len(mat)-1)

def rdpair(mat):
  return rdind(mat), rdind(mat) 

def rdcoeff():
  return randint(-MAXCOEFF, MAXCOEFF)

def scramble_matrix(mat, det):
  for i in range(MAXSCRAMBLES*len(mat)):
    r1, r2 = rdpair(mat)
    if r1 != r2:
      swap_row(mat, r1, r2)
      det = -det

    c1, c2 = rdpair(mat)
    if c1 != c2:
      swap_column(mat, c1, c2)
      det = -det

    r1, r2 = rdpair(mat)
    if r1 != r2:
      add_row(mat, rdcoeff(), r1, r2)

    c1, c2 = rdpair(mat)
    if c1 != c2:
      add_column(mat, rdcoeff(), c1, c2)

  return mat, det
  

def generate_scrambled_matrix(sz, det):
  mat = create_identity_matrix(sz)
  mat[0][0] = det
  mat, det = scramble_matrix(mat, det)

  return mat, det


def read_and_scramble(filename):
  matrices = []
  with open(filename, "r") as f:
    for line in f.readlines():
      sz, det = map(float, line.split())
      matrices.append(generate_scrambled_matrix(sz, det))
  return matrices

def gen_scrambled(filename):
  tests = []
  for mat, det in read_and_scramble(filename):
    tests.append(gen_test(mat, det))
  return tests


def parse_hardcoded_line(line):
  dim, val, res = line.split(":")
  dim = int(dim)
  res = float(res)
  
  val = [v for v in val.split() if v != '']

  val = list(map(float, val))

  return dim, val, res

def gen_hardcoded(filename):
  tests = []
  with open(filename, "r") as f:
    for line in f.readlines():
      dim, val, res = parse_hardcoded_line(line)
      tests.append(gen_hardcoded_test(dim, val, res))
  return tests

if __name__ == "__main__":
  
  with open("build.test", "w") as f:
    tests = gen_hardcoded("hardcoded.test")
    tests.extend(gen_scrambled("scrambled.test"))
    f.write(f"{len(tests)}\n")
    for test in tests:
      f.write(test + "\n")