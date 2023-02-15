#include "task.hpp"

namespace HW3D
{

Triangle3D read_triangle(std::istream& stream)
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

  stream >> x1 >> y1 >> z1;
  assert(stream.good());

  stream >> x2 >> y2 >> z2;
  assert(stream.good());

  stream >> x3 >> y3 >> z3;
  assert(stream.good());

  Point3D p1 {x1, y1, z1};
  Point3D p2 {x2, y2, z2};
  Point3D p3 {x3, y3, z3};

  try
  {
    Triangle3D tr {p1, p2, p3};
    return tr;
  }
  catch (std::invalid_argument)
  {
    throw std::runtime_error("Attempt to create triangle with two or more coincident points");
  }
}

std::list<PointsEntry> read_triangles(std::istream& stream)
{
  size_t N = 0;
  stream >> N;
  assert(stream.good());

  std::list<PointsEntry> triangles;
  for (int i = 0; i < N; i++) {
    PointsEntry entry {read_triangle(stream).simplify(), i};
    triangles.push_back(std::move(entry));
  }

  return triangles;
}

std::vector<int> count_intersections(std::list<PointsEntry>& triangles)
{
  size_t N = triangles.size();
  std::vector<int> intersections;

  auto p = triangles.begin();

  while(p != triangles.end())
  {
    auto q  = std::next(p);
    bool in = false; // Shows if p will go to out

    while(q != triangles.end())
    {
      auto next = std::next(q);
      if (intersect(p->pts, q->pts))
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

  for (int i = 0; i < N; ++i)
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