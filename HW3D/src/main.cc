#include "task_algorithm.hpp"
#include <iostream>
#include <cassert>

using namespace HW3D;

Triangle3D read_triangle()
{
  double x1 = NAN;
  double y1 = NAN;
  double z1 = NAN;

  double x2 = NAN;
  double y2 = NAN;
  double z2 = NAN;

  double x3 = NAN;
  double y3 = NAN;
  double z3 = NAN;

  std::cin >> x1 >> y1 >> z1;
  assert(std::cin.good());

  std::cin >> x2 >> y2 >> z2;
  assert(std::cin.good());

  std::cin >> x3 >> y3 >> z3;
  assert(std::cin.good());

  Point3D p1 {x1, y1, z1};
  Point3D p2 {x2, y2, z2};
  Point3D p3 {x3, y3, z3};

  return {p1, p2, p3};
}

int main()
{
  size_t N = 0;
  std::cin >> N;
  assert(std::cin.good());
  std::vector<Triangle3D> triangles;

  for (int i = 0; i < N; i++) {
    triangles.push_back(read_triangle());
  }

  for (int i = 0; i < N; i++)
  {
    for (int k = 0; k < N; k++)
    {
      if (k == i) continue;
      if (intersect_triangles(triangles[i], triangles[k]))
      {
        std::cout << i << std::endl;
        break;
      }

    }
  }

  return 0;
}