#include "task_algorithm.hpp"

namespace HW3D
{

// Reference: [GCT page 265]
bool intersect_complanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept
{
  auto normals1 = get_edge_normals(tr1);
  auto normals2 = get_edge_normals(tr2);

  for (int i = 0; i < 3; i++)
  {
    auto interval1 = ComputeInterval(tr1, normals1[i]);
    auto interval2 = ComputeInterval(tr2, normals1[i]);

    double max1 = interval1[1];
    double min1 = interval1[0];

    double max2 = interval2[1];
    double min2 = interval2[0];

    if (!intervals_intersect(min1, max1, min2, max2))
      return false;
  }

  for (int i = 0; i < 3; i++)
  {
    auto interval1 = ComputeInterval(tr1, normals2[i]);
    auto interval2 = ComputeInterval(tr2, normals2[i]);

    double max1 = interval1[1];
    double min1 = interval1[0];

    double max2 = interval2[1];
    double min2 = interval2[0];

    if (!intervals_intersect(min1, max1, min2, max2))
      return false;
  }

  return true;
}

std::array<double,2> ComputeInterval(const Triangle3D& tr, const Vec3D& direction)
{
  assert(direction != (Vec3D{0, 0, 0}));

  std::vector<double> projections;

  for (int i = 0; i < 3; i++)
  {
    Point3D v = tr.get_vertice(i);
    double t = v * direction; 
    projections.push_back(t);
  }

  auto minmax = std::minmax_element(projections.begin(), projections.end());

  return {*minmax.first, *minmax.second}; 
}

// Reference: [GCT page 541]
bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();

  PlaneRelation pi = get_plane_relation(p1, p2);

  LineInf3D pil = get_plane_intersection(p1, p2, pi);

  std::vector<double> intrs1 = intersect_with(tr1, pil);
  std::vector<double> intrs2 = intersect_with(tr2, pil);

  // If triangle intersects infinite pil line, intersection count can be zero, two or three
  if (intrs1.size() < 2 || intrs2.size() < 2)
    return false;

  double max1 = intrs1.back();
  double min1 = intrs1.front();

  double max2 = intrs2.back();
  double min2 = intrs2.front();

  // Next we shold apply interval overlap method
  return intervals_intersect(min1, max1, min2, max2);
}

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();

  // Intersect triangles' planes
  PlaneRelation pi = get_plane_relation(p1, p2);

  switch(pi.get_state())
  {
    case pi.PARALLEL:
    {
      return false;
    }

    case pi.COINCIDENT:
    {
      return intersect_complanar_triangles(tr1, tr2);
    }
    
    case pi.INTERSECTING:
    {
      return intersect_noncomplanar_triangles(tr1, tr2);
    }
  } 

  return false;
}


}