#include "task.hpp"

namespace HW3D
{

Point3D read_point(std::istream& stream)
{
  data_t x, y, z = NAN;
  stream >> x >> y >> z;
  assert(stream.good());
  return {x, y, z};
}

Triangle3D read_triangle(std::istream& stream)
{
  Point3D p1 = read_point(stream);
  Point3D p2 = read_point(stream);
  Point3D p3 = read_point(stream);

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