/* HW3D lvl1-specific functionality implementation */
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

std::unique_ptr<IIntersectible> read_object(std::istream& stream)
{
  Point3D p1 = read_point(stream);
  Point3D p2 = read_point(stream);
  Point3D p3 = read_point(stream);

  std::array<Point3D, 3> points = {p1, p2, p3};
  return IntersectibleFactory::create(points);
}

std::vector<PointsEntry> read_objects(std::istream& stream)
{
  size_t N = 0;
  stream >> N;
  assert(stream.good());

  std::vector<PointsEntry> triangles;
  for (size_t i = 0; i < N; i++)
    triangles.push_back({read_object(stream), i});

  return triangles;
}

std::vector<int> count_intersections(std::vector<PointsEntry>& triangles)
{
  std::vector<int> intersections;
  
  for(auto p = triangles.begin(); p != triangles.end(); p = std::next(p))
  {
    if (p->in) continue;

    for(auto q = std::next(p); q != triangles.end(); q = std::next(q))
    {  
      if (intersects(*p->object, *q->object))
      {
        intersections.push_back(p->n);

        if (!q->in)
        {
          intersections.push_back(q->n);
          q->in = true;
        }
        break;
      }
    }
  }

  std::sort(intersections.begin(), intersections.end());
  return intersections;
}

void write_intersections(std::ostream& stream, const std::vector<int>& intersections)
{
  size_t N = intersections.size();

  for (size_t i = 0; i < N; ++i)
  {
    stream << intersections[i] << std::endl;
  }

  stream << std::endl;
}

void task_e2e(std::istream& in, std::ostream& out)
{
  auto triangles     = read_objects(in);
  auto intersections = count_intersections(triangles);

  write_intersections(out, intersections);
}

}