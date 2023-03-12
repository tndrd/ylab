from testgen import UNIT_TEST_FILE
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib as mpl
mpl.rcParams['figure.dpi'] = 300
mpl.rcParams["animation.html"] = "jshtml"
mpl.rcParams["animation.embed_limit"] = 100

COLORS = ["b", "g", "r", "c", "m", "y", "k"]
COLOR_COUNTER = 0

def GET_COLOR():
  global COLORS
  global COLOR_COUNTER

  color = COLORS[COLOR_COUNTER]
  COLOR_COUNTER = (COLOR_COUNTER + 1) % len(COLORS)

  return color


def load_triangle(line):
  line = line.split()
  line = list(map(float, line))

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
  
  return tr1

def load_triangles(lines):
  triangles = []
  for line in lines:
    triangles.append(load_triangle(line))

  return triangles

def draw_triangle(subplot, triangle, color, label):
  triangle[0].append(triangle[0][0])
  triangle[1].append(triangle[1][0])
  triangle[2].append(triangle[2][0])
  
  return subplot.plot(triangle[0], triangle[1], triangle[2], f"{color}o-", label=label)[0]

def draw_triangles(triangles):
  fig = plt.figure(figsize=(20,20))
  subplot = fig.add_subplot(1, 1, 1, projection='3d')
  
  for i in range(len(triangles)):
    draw_triangle(subplot, triangles[i], GET_COLOR(), f"#{i}")
  
  subplot.legend()
  plt.show()

if __name__ == "__main__":

  path = input("Specify path: ")
  if path == "": path = "issue.txt"

  with open(path, "r") as f:
    triangles = load_triangles(f.readlines()[1:])
    draw_triangles(triangles)

