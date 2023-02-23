#include "task.hpp"

namespace HW3D
{

Triangle3D read_triangle(std::istream& stream)
{
  data_t x1, y1, z1 = NAN;
  data_t x2, y2, z2 = NAN;
  data_t x3, y3, z3 = NAN;

  stream >> x1 >> y1 >> z1;
  assert(stream.good());

  stream >> x2 >> y2 >> z2;
  assert(stream.good());

  stream >> x3 >> y3 >> z3;
  assert(stream.good());

  Point3D p1 {x1, y1, z1};
  Point3D p2 {x2, y2, z2};
  Point3D p3 {x3, y3, z3};

  Triangle3D tr {p1, p2, p3};
  return tr;
}

std::list<PointsEntry> read_triangles(std::istream& stream)
{
  size_t N = 0;
  stream >> N;
  assert(stream.good());

  std::list<PointsEntry> triangles;
  for (size_t i = 0; i < N; i++)
    triangles.push_back({{read_triangle(stream)}, i});

  return triangles;
}

std::vector<int> count_intersections(std::list<PointsEntry>& triangles)
{
  std::vector<int> intersections;

  auto p = triangles.begin();

  while(p != triangles.end())
  {
    auto q  = std::next(p);
    bool in = false; // Shows if p will go to out

    while(q != triangles.end())
    {
      auto next = std::next(q);
      if (intersect(p->pgroup, q->pgroup))
      {
        if(!in)
        {
          intersections.push_back(p->n);
          in = true;
        }
        intersections.push_back(q->n);
        triangles.erase(q);
      }
      q = next;
    }

    p = std::next(p);
  }

  return intersections;
}

void write_intersections(std::ostream& stream, const std::vector<int>& intersections)
{
  size_t N = intersections.size();

  for (size_t i = 0; i < N; ++i)
  {
    stream << intersections[i] << " ";
  }

  stream << std::endl;
}

void task_e2e(std::istream& in, std::ostream& out)
{
  auto triangles     = read_triangles(in);
  auto intersections = count_intersections(triangles);

  write_intersections(out, intersections);
}

}