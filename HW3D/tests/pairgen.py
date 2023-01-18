from random import randint, sample
import numpy as np
from math import sin, cos, sqrt

BASE_LEN = 5
RANDPOINT_BORDER = 2 * BASE_LEN
RANDPOINT_COEFF = 100
PERIMETER_TO_AREA_TOLERANCE = ((2 + sqrt(2)) / BASE_LEN) / 3
A4_OFFSET = BASE_LEN * 0.1

LBORDER = -RANDPOINT_BORDER + BASE_LEN / 2
RBORDER =  RANDPOINT_BORDER + BASE_LEN / 2
TBORDER =  RANDPOINT_BORDER + BASE_LEN * 2
BBORDER = -RANDPOINT_BORDER - BASE_LEN

ZBORDER_T = RANDPOINT_BORDER
ZBORDER_B = -RANDPOINT_BORDER
ORTH_OFFSET = ZBORDER_T * 0.2
DCOEFF = (RBORDER - LBORDER) * sqrt(2) / 2

DIRECTION_LEN_MAX = 2

B4_RANGE  = (ZBORDER_T - ZBORDER_B) / 2
B4_OFFSET = (ZBORDER_T - ZBORDER_B) * 0.1

PP2_OFFSET = (ZBORDER_T - ZBORDER_B) * 0.1
PL2_OFFSET = (ZBORDER_T - ZBORDER_B) * 0.1

LL1_COEFF = 2

PT2_2_OFFSET = (ZBORDER_T - ZBORDER_B) * 0.1

LT1_COEFF = 2
LT2_COEFF = 2

def xlim():
  return (LBORDER, RBORDER)

def ylim():
  return (BBORDER, TBORDER)

def zlim():
  return (ZBORDER_B, ZBORDER_T)

def create_base():
  a = (0, 0, 0)
  b = (BASE_LEN, 0, 0)
  c = (0, BASE_LEN, 0)
  return np.array([a, b, c])

def rand(a, b):
  return a + randint(0, int((b - a) * RANDPOINT_COEFF)) / RANDPOINT_COEFF

def randsign():
  return 1 - 2 * randint(0, 1)

def randx():
  return rand(LBORDER, RBORDER)

def randy():
  return rand(BBORDER, TBORDER)

def randz():
  return rand(ZBORDER_B, ZBORDER_T)

def randpoint():
  return np.array([randx(), randy(), randz()])

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

def angle_from_seg(nseg):
  angle = None
  if nseg == 1: angle = from_seg1() 
  if nseg == 2: angle = from_seg2()
  if nseg == 3: angle = from_seg3()
  return angle 

def generate_A3_point(n_seg):
  return base_circum_point(angle_from_seg(n_seg))

def generate_intersection_segments():
  segments = set([1, 2, 3])

  seg_a = sample(segments, 1)[0]
  seg_b = sample(segments, 1)[0]
  segments.remove(seg_a)
  seg_c = sample(segments, 1)[0]

  return seg_a, seg_b, seg_c

def A3():
  seg_a, seg_b, seg_c = generate_intersection_segments()

  point_a = generate_A3_point(seg_a)
  point_b = generate_A3_point(seg_b)
  point_c = generate_A3_point(seg_c)

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

def A4():
  n = randint(1, 6)
  if n == 1: return A4_1_1()
  if n == 2: return A4_1_2()
  if n == 3: return A4_2_1()
  if n == 4: return A4_2_2()
  if n == 5: return A4_3_1()
  if n == 6: return A4_3_2()

def length(vec):
  return sqrt(np.dot(vec, vec))

def normalize(vec):
  return vec / length(vec)

def orth_point(pa, pb):

  norm = np.cross(pa, pb)
  norm = normalize(norm)

  direction = pb - pa
  direction = normalize(direction)

  zcoeff = (1 - 2 * randint(0,1)) * rand(ORTH_OFFSET, ZBORDER_T)
  
  dcoeff = rand(0, DCOEFF)

  point = pa + norm * zcoeff + direction * dcoeff
  return point

def rand_line_seg(p1, p2):
  direction = p2 - p1
  direction_l = length(direction)
  point = p1 + normalize(direction) * rand(direction_l, direction_l * DIRECTION_LEN_MAX)
  return point  

def orth_triangle(p1, p2):
  point_a = orth_point(p1,p2)
  point_b = rand_line_seg(point_a, p1)
  point_c = rand_line_seg(point_a, p2)

  triangle = np.array([point_a, point_b, point_c]) 
  return triangle, (p1, p2)

def B1():
  p1, p2, _ = A4()
  return orth_triangle(p1, p2)

def B2():
  seg_a, seg_b = sample(set([1, 2, 3]), 2)

  p1 = generate_A3_point(seg_a)
  p2 = generate_A3_point(seg_b)

  return orth_triangle(p1, p2)

def B3():
  p1 = point_in_base()
  p2 = base_plane_point()
  return orth_triangle(p1, p2)

def B4():
  offset = randsign() * rand(B4_OFFSET, B4_RANGE)

  p1 = base_plane_point()
  p2 = base_plane_point()
  p3 = base_plane_point()

  p1[2], p2[2], p3[2] = offset, offset, offset

  p0 = np.array([0, 0, 0])
  return np.array([p1, p2, p3]), (p0, p0)

def PP1():
  p1 = randpoint()
  return np.array([p1, p1, p1]), np.array([p1, p1, p1]) 

def PP2():
  p1 = randpoint()
  p2 = randpoint()
  while (length(p1 - p2) < PP2_OFFSET):
    p2 = randpoint()

  return np.array([p1, p1, p1]), np.array([p2, p2, p2]) 

def randbetween(p1, p2):
  return p1 + (p2 - p1) * rand(0,1)

def gen_base_line():
  p1 = np.array([0, 0, 0])
  p2 = np.array([BASE_LEN, 0, 0])
  p3 = randbetween(p1, p2)
  return np.array([p1, p2, p3])

def base_line_point():
  x = rand(0, BASE_LEN)
  return np.array([x, 0, 0])

def PL1():
  line = gen_base_line()
  pt = base_line_point()

  return line, np.array([pt, pt, pt])

def check_PL2(pt):
  if abs(pt[1]) < PL2_OFFSET and (-PL2_OFFSET) < pt[0] < (BASE_LEN - PL2_OFFSET):
    return False
  return True

def PL2():
  line = gen_base_line()
  pt = base_plane_point()
  while not check_PL2(pt):
    pt = base_plane_point()
  return line, np.array([pt, pt, pt]) 

def LL1():
  line = gen_base_line()
  pt = base_line_point()

  p1 = base_plane_point()
  p2 = p1 + (pt - p1) * rand(1, LL1_COEFF)
  p3 = randbetween(p1, p2)

  return line, np.array([p1, p2, p3])

def LL2():
  line = gen_base_line()
  z = randz()
  p1, p2, _ = A4()
  p1[2] = z
  p2[2] = z
  p3 = randbetween(p1,p2)
  
  return line, np.array([p1, p2, p3])

def PT1():
  base = create_base()
  pt = point_in_base()
  return base, np.array([pt, pt, pt])

def generate_PT2_1_point():
  pt = A4()[0]
  pt[2] = randz()
  return pt

def generate_PT2_2_point():
  pt = point_in_base()
  pt[2] = randz()
  while abs(pt[2]) < PT2_2_OFFSET:
    pt[2] = randz()
  return pt

def PT2():
  base = create_base()
  n = randint(1, 2)
  pt = None
  if n == 1: pt = generate_PT2_1_point()
  if n == 2: pt = generate_PT2_2_point()
  return base, np.array([pt, pt, pt])

def LT1():
  base = create_base()
  pt = point_in_base()
  p1 = randpoint()
  p2 = p1 + (pt - p1) * rand(1, LT1_COEFF)
  p3 = randbetween(p1, p2)
  return base, np.array([p1, p2, p3])

def LT2():
  base = create_base()
  pt = A4()[0]
  p1 = randpoint()
  p2 = p1 + (pt - p1) * rand(0.5, LT2_COEFF)
  p3 = randbetween(p1, p2)
  return base, np.array([p1, p2, p3])

def gen3D(gen_foo):
  triangle, info = gen_foo()
  while not validate_triangle(triangle):
    triangle, info = gen_foo()
  return triangle, info  

def gen(gen_foo):
  triangle = gen_foo()
  while not validate_triangle(triangle):
    triangle = gen_foo()
  return triangle

def randpair2D():
  base = create_base()
  gen_foo   = None
  intersect = None
  n = randint(1,4)

  if n == 1: gen_foo, intersect = A1, True
  if n == 2: gen_foo, intersect = A2, True
  if n == 3: gen_foo, intersect = A3, True
  if n == 4: gen_foo, intersect = A4, False
  
  return intersect, base, gen(gen_foo) 

def randdegenerate():
  n = randint(0,9)
  triangles = None
  intersect = None
  
  generators = [PP1, PP2, PL1, PL2, LL1, LL2, PT1, PT2, LT1, LT2]
  intersects = [True, False, True, False, True, False, True, False, True, False]
  return intersects[n], *generators[n]()


def randpair3D():
  return randdegenerate()
  base = create_base()
  gen_foo   = None
  intersect = None
  n = randint(1,8)

  if n == 1: gen_foo, intersect = A1, True
  if n == 2: gen_foo, intersect = A2, True
  if n == 3: gen_foo, intersect = A3, True
  if n == 4: gen_foo, intersect = A4, False
  if n == 5: gen_foo, intersect = B1, False
  if n == 6: gen_foo, intersect = B2, True
  if n == 7: gen_foo, intersect = B3, True
  if n == 8: gen_foo, intersect = B4, False
  
  triangle = None
  if n < 5:
    triangle = gen(gen_foo)
  else:
    triangle, _ = gen3D(gen_foo)

  return intersect, base, triangle 
