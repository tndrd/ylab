from testgen import TESTFILE
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib as mpl
mpl.rcParams['figure.dpi'] = 300
mpl.rcParams["animation.html"] = "jshtml"
mpl.rcParams["animation.embed_limit"] = 100

def load_triangles(line):
  line = line.split()
  test_n = int(line[0])
  result = bool(line[-1])

  line = list(map(float, line[1:-1]))

  xs = []
  ys = []
  zs = []

  for i in range(0, len(line), 3):
    xs.append(line[i])

  for i in range(1, len(line), 3):
    ys.append(line[i])

  for i in range(2, len(line), 3):
    zs.append(line[i])

  tr1 = (xs[0:3], ys[0:3], zs[0:3])
  tr2 = (xs[3:6], ys[3:6], zs[3:6])

  print(f"Loaded test #{test_n}, intersection: {result}")
  
  return tr1, tr2

def draw_triangle(subplot, triangle, color):
  triangle[0].append(triangle[0][0])
  triangle[1].append(triangle[1][0])
  triangle[2].append(triangle[2][0])
  
  return subplot.plot(triangle[0], triangle[1], triangle[2], color=color)[0]

def draw_test(triangles):
  fig = plt.figure(figsize=(20,20))
  subplot = fig.add_subplot(1, 1, 1, projection='3d')
  tr1 = draw_triangle(subplot, triangles[0], "red")
  tr2 = draw_triangle(subplot, triangles[1], "orange")
  plt.show()

if __name__ == "__main__":
  with open(TESTFILE, "r") as f:
    test_n = int(input("Enter test number: "))
    line = f.readlines()[test_n - 1]
    triangles = load_triangles(line)
    print("Rendering...", end=" ")
    draw_test(triangles)
    print("Done")

