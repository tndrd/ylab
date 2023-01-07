from pairgen import *
from math import acos
import quaternion as quat

POINT_STRETCH_MIN = 0.5
POINT_STRETCH_MAX = 2

LINE_POINT_COEFF = 0.2
LINE_STRETCH_MIN = 0.2
LINE_STRETCH_MAX = 1 / LINE_STRETCH_MIN

PLANE_POINT_COEFF = 0.2
PLANE_STRETCH_MIN = 0.2
PLANE_STRETCH_MAX = 1 / LINE_STRETCH_MIN

# POINT STRETCH

def point_point_stretch(stretchpoint, point, coeff):
  return stretchpoint + (point - stretchpoint) * coeff

def triangle_point_stretch(stretchpoint, triangle, coeff):
  a0, b0, c0 = triangle

  a = point_point_stretch(stretchpoint, a0, coeff)
  b = point_point_stretch(stretchpoint, b0, coeff)
  c = point_point_stretch(stretchpoint, c0, coeff)

  new_triangle = np.array([a, b, c])
  return new_triangle

def point_stretch_coeff():
  return rand(POINT_STRETCH_MIN, POINT_STRETCH_MAX)

def point_stretch2D(tr1, tr2):
  stretchpoint = base_plane_point()
  coeff = point_stretch_coeff()

  new_tr1 = triangle_point_stretch(stretchpoint, tr1, coeff)
  new_tr2 = triangle_point_stretch(stretchpoint, tr2, coeff)

  return new_tr1, new_tr2

def point_stretch(tr1, tr2):
  stretchpoint = randpoint()
  coeff = point_stretch_coeff()

  new_tr1 = triangle_point_stretch(stretchpoint, tr1, coeff)
  new_tr2 = triangle_point_stretch(stretchpoint, tr2, coeff)

  return new_tr1, new_tr2

# LINE STRETCH

def randline():
  p = randpoint() * LINE_POINT_COEFF
  a = normalize(randpoint())
  p = p - np.dot(p, a) * a

  return np.array([a, p])

def point_line_stretch(stretchline, point, coeff):
  a, p = stretchline
  q = p - (np.dot((p - point), a)) * a

  newpoint = q + (point - q) * coeff

  newpoint = point_point_stretch((0, 0, 0), newpoint, 1 / coeff)
  return newpoint


def triangle_line_stretch(stretchline, triangle, coeff):
  a0, b0, c0 = triangle

  a = point_line_stretch(stretchline, a0, coeff)
  b = point_line_stretch(stretchline, b0, coeff)
  c = point_line_stretch(stretchline, c0, coeff)

  new_triangle = np.array([a, b, c])
  return new_triangle

def line_stretch_coeff():
  return randsign() * rand(LINE_STRETCH_MIN, LINE_STRETCH_MAX)

def line_stretch(tr1, tr2):
  stretchline = randline()
  coeff = line_stretch_coeff()

  new_tr1 = triangle_line_stretch(stretchline, tr1, coeff)
  new_tr2 = triangle_line_stretch(stretchline, tr2, coeff)

  return new_tr1, new_tr2

# PLANE STRETCH

def randplane():
  p = randx() * PLANE_POINT_COEFF 
  n = normalize(randpoint())
  return np.array([n, p])

def point_plane_stretch(stretchplane, point, coeff):
  n, p = stretchplane
  q = point + (p - np.dot(point, n)) * n

  newpoint = q + (point - q) * coeff
  newpoint = point_point_stretch((0, 0, 0), newpoint, 1 / coeff)
  return newpoint

def triangle_plane_stretch(stretchplane, triangle, coeff):
  a0, b0, c0 = triangle

  a = point_plane_stretch(stretchplane, a0, coeff)
  b = point_plane_stretch(stretchplane, b0, coeff)
  c = point_plane_stretch(stretchplane, c0, coeff)

  new_triangle = np.array([a, b, c])
  return new_triangle

def plane_stretch_coeff():
  return randsign() * rand(PLANE_STRETCH_MIN, PLANE_STRETCH_MAX)

def plane_stretch(tr1, tr2):
  stretchplane = randplane()
  coeff = plane_stretch_coeff()

  new_tr1 = triangle_plane_stretch(stretchplane, tr1, coeff)
  new_tr2 = triangle_plane_stretch(stretchplane, tr2, coeff)

  return new_tr1, new_tr2

# ROTATION

def randquat():
  q = normalize(np.array([rand(-1, 1) for i in range(4)]))
  return quat.quaternion(*q)

def point_rotation(quaternion, point):
  v = np.array([0.] + point)
  v = quat.quaternion(*v)

  new_point = quaternion * v * np.conjugate(quaternion)
  return new_point.imag

def triangle_rotation(quaternion, triangle):
  a0, b0, c0 = triangle

  a = point_rotation(quaternion, a0)
  b = point_rotation(quaternion, b0)
  c = point_rotation(quaternion, c0)

  new_triangle = np.array([a, b, c])
  return new_triangle

def rotation(tr1, tr2):
  quaternion = randquat()

  new_tr1 = triangle_rotation(quaternion, tr1)
  new_tr2 = triangle_rotation(quaternion, tr2)

  return new_tr1, new_tr2

# GENERAL

def triangle_normal(tr):
  a = tr[1] - tr[0]
  b = tr[2] - tr[0]

  return normalize(np.cross(a, b))  

def angle_between(tr1, tr2):
  n1 = triangle_normal(tr1)
  n2 = triangle_normal(tr2)

  m = abs(np.dot(n1, n2))
  if m > 1: m = 1

  angle = acos(m) / 3.14 * 180
  
  return angle

def baseangle(tr):
  side1 = normalize(tr[1] - tr[0])
  side2 = normalize(tr[2] - tr[0]) 
  return acos(np.dot(side1, side2)) / 3.14 * 180

def scramble(tr1, tr2, scramble_foo):
  new_tr1, new_tr2 = scramble_foo(tr1, tr2)

  #print(f"Angle was {angle_between(tr1, tr2)}, {baseangle(tr1)}")
  #print(f"Angle got {angle_between(new_tr1, new_tr2)}, {baseangle(new_tr1)}")

  return new_tr1, new_tr2

def randscramble(tr1, tr2):
  n_scramble   = randint(0, 3)
  scramble_foo = None

  if n_scramble == 0: scramble_foo = point_stretch
  if n_scramble == 1: scramble_foo = line_stretch
  if n_scramble == 2: scramble_foo = plane_stretch
  if n_scramble == 3: scramble_foo = rotation

  new_tr1, new_tr2 = scramble(tr1, tr2, scramble_foo)
  return new_tr1, new_tr2