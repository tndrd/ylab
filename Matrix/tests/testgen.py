
def gen_test(dim, val, res, suite, name):
  print(f"TEST({suite}, {name})")
  print("{")
  print(f"  std::vector<double> data ", end=" ")
  print_val(val)
  print(";")
  print(f"  Matrix<double, {dim}, {dim}> mat = data;")
  print(f"  EXPECT_FIT(det(mat), {res});")
  print("}")

def print_val(val):
  print("{", end=" ")
  for i in range(len(val) - 1):
    print(f'{val[i]}, ', end=" ")
  print(val[-1], end=" ")
  print("}", end="")

def parse_det_test_line(line):
  dim, val, res = line.split(":")
  dim = int(dim)
  res = float(res)
  
  val = [v for v in val.split() if v != '']

  val = list(map(float, val))

  return dim, val, res

with open("determinant.test", "r") as f:
  i = 1
  for line in f.readlines():
    dim, val, res = parse_det_test_line(line)
    gen_test(dim, val, res, "Matrix", f"Determinant{i}")
    print("\n")
    i += 1