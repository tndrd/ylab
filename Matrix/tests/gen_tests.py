from random import randint
from math import log
MAXSIZE  = 100
MAXDET   = 100

def gen(quantity):
  tests = []
  print(f"Generating {quantity} scrambled matrices")
  for i in range(quantity):
    size = randint(1, MAXSIZE)
    det  = randint(-MAXDET, MAXDET)
    test = f"{size} {det}\n"
    tests.append(test)

  zerodet_quantity = int(log(quantity)) + 5 
  print(f"Generating {zerodet_quantity} scrambled degenerate matrices")
  for i in range(zerodet_quantity):
    size = randint(1, MAXSIZE)
    det  = 0
    test = f"{size} {det}\n"
    tests.append(test)

  return tests


if __name__ == "__main__":
  with open("scrambled.test", "w") as f:
    quantity = int(input())
    tests = gen(quantity)

    for test in tests:
      f.write(test)

    f.write("100 42")