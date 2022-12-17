from random import randint

MAXSIZE  = 100
MAXDET   = 100

def gen(quantity):
  tests = []

  for i in range(quantity):
    size = randint(1, MAXSIZE)
    det  = randint(-MAXDET, MAXDET)
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