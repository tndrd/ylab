from random import randint, sample
import numpy as np
from math import sin, cos

BASE_LEN = 5
RANDPOINT_BORDER = 2 * BASE_LEN
RANDPOINT_COEFF = 100
PERIMETER_TO_AREA_TOLERANCE = ((2 + 2**0.5) / BASE_LEN) / 3
A4_OFFSET = BASE_LEN * 0.1
A3_OFFSET = BASE_LEN * 0.3

LBORDER = -RANDPOINT_BORDER + BASE_LEN / 2
RBORDER =  RANDPOINT_BORDER + BASE_LEN / 2
TBORDER =  RANDPOINT_BORDER + BASE_LEN * 2
BBORDER = -RANDPOINT_BORDER - BASE_LEN

def xlim():
  return (LBORDER, RBORDER)

def ylim():
  return (BBORDER, TBORDER)

def create_base():
  a = (0, 0, 0)
  b = (BASE_LEN, 0, 0)
  c = (0, BASE_LEN, 0)
  return np.array([a, b, c])

def rand(a, b):
  return a + randint(0, int((b - a) * RANDPOINT_COEFF)) / RANDPOINT_COEFF

def randx():
  return rand(LBORDER, RBORDER)

def randy():
  return rand(BBORDER, TBORDER)

def base_plane_point():
  return np.array([randx(), randy(), 0])

def validate_triangle(triangle):
  vec1 = triangle[1] - triangle[0]
  vec2 = triangle[2] - triangle[0]
  vec3 = triangle[2] - triangle[1]
  cross = np.cross(vec1, vec2)
  area = abs(np.dot(cross, cross))
  perim = np.dot(vec1, vec1) + np.dot(vec2, vec2) + np.dot(vec3, vec3)
  
  if not area:
    return False
  if perim / area > PERIMETER_TO_AREA_TOLERANCE:
    return False
  return True 

def point_in_base():
  x = rand(0, BASE_LEN)
  y = rand(0, BASE_LEN - x)
  return np.array([x, y, 0])

def A1():
  a = point_in_base()
  b = base_plane_point()
  c = base_plane_point()

  triangle = np.array([a, b, c])
  return triangle

def A2():
  a = point_in_base()
  b = point_in_base()
  c = base_plane_point()

  triangle = np.array([a, b, c])
  return triangle


def from_seg1():
  return rand(-np.pi * 1/4, np.pi * 3/4)

def from_seg2():
  return rand(np.pi * 3/4, np.pi * 5/4)

def from_seg3():
  return rand(np.pi * 5/4, np.pi * 7/4)

def base_circum_point(angle):
  x = BASE_LEN / 2 + cos(angle) * BASE_LEN * 2**(-0.5)
  y = BASE_LEN / 2 + sin(angle) * BASE_LEN * 2**(-0.5)
  return np.array([x, y, 0])

def point_from_seg(nseg):
  angle = None
  if nseg == 1: angle = from_seg1() 
  if nseg == 2: angle = from_seg2()
  if nseg == 3: angle = from_seg3()
  return base_circum_point(angle) 

def A3():
  segments = set([1, 2, 3])

  seg_a = sample(segments, 1)[0]
  seg_b = sample(segments, 1)[0]
  segments.remove(seg_a)
  seg_c = sample(segments, 1)[0]

  point_a = point_from_seg(seg_a)
  point_b = point_from_seg(seg_b)
  point_c = point_from_seg(seg_c)

  return np.array([point_a, point_b, point_c])
  
def generate_A4_1_1_point():
  x = rand(LBORDER + A4_OFFSET, RBORDER)
  y0 = BASE_LEN - x + A4_OFFSET
  y = TBORDER
  if y0 > TBORDER: raise BaseException
  y = rand(y0, TBORDER)
  return np.array([x, y, 0])

def generate_A4_1_2_point():
  x = rand(LBORDER, RBORDER - A4_OFFSET)
  y0 = - x - A4_OFFSET
  y = BBORDER
  if y0 < BBORDER: raise BaseException
  y = rand(BBORDER, y0)
  return np.array([x, y, 0])  

def generate_A4_2_1_point():
  x = randx()
  y = rand(BBORDER, -A4_OFFSET)
  return np.array([x, y, 0])

def generate_A4_2_2_point():
  x = randx()
  y = rand(BASE_LEN + A4_OFFSET, TBORDER)
  return np.array([x, y, 0])

def generate_A4_3_1_point():
  y = randy()
  x = rand(LBORDER, -A4_OFFSET)
  return np.array([x, y, 0])

def generate_A4_3_2_point():
  y = randy()
  x = rand(BASE_LEN + A4_OFFSET, RBORDER)
  return np.array([x, y, 0])

def gen_template(point_gen):
  a = point_gen()
  b = point_gen()
  c = point_gen()

  triangle = np.array([a, b, c])
  return triangle

def A4_1_1():
  return gen_template(generate_A4_1_1_point)

def A4_1_2():
  return gen_template(generate_A4_1_2_point)

def A4_2_1():
  return gen_template(generate_A4_2_1_point)

def A4_2_2():
  return gen_template(generate_A4_2_2_point)

def A4_3_1():
  return gen_template(generate_A4_3_1_point)

def A4_3_2():
  return gen_template(generate_A4_3_2_point)

def gen(gen_foo):
  triangle = gen_foo()
  while not validate_triangle(triangle):
    triangle = gen_foo()
  return triangle