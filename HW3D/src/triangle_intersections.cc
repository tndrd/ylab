#include "triangle_intersections.hpp"

namespace HW3D
{

int distsign(double dist) noexcept
{
  return (fit(dist, 0)) ? 0 : 1 - (2 * std::signbit(dist));
  //return std::signbit(dist);
}

int find_separate_point(const std::array<int, 3>& signs) noexcept
{
  for (int i = 0; i < 3; ++i)
  {
    int s_i   = signs[i];
    int s_i_r = signs[(i+1)%3];
    int s_i_l = signs[(i+2)%3];
    
    bool cond1 = (s_i != s_i_r) || !s_i_r;
    bool cond2 = (s_i != s_i_l) || !s_i_l;

    if (s_i && cond1 && cond2)
    {
      return i;
    }
  }
  return -1;
}

std::array<int, 3> get_dist_signs(const std::array<double, 3>& distances) noexcept
{
  std::array<int, 3> signs;
  for (int i = 0; i < 3; ++i)
  {
    signs[i] = distsign(distances[i]);
  }
  return signs;
}

std::array<double, 3> get_projections(const Triangle3D& tr, const LineInf3D& line) noexcept
{
  std::array<double, 3> projections;
  for (int i = 0; i < 3; i++)
  { 
    double V = line.get_a() * (tr.get_vertice(i) - line.get_p());
    projections[i] = V;
  }
  return projections;
}

std::array<double, 3> get_distances(const Triangle3D& tr, const Plane3D& plane) noexcept
{
  std::array<double, 3> distances;
  for (int i = 0; i < 3; ++i)
  {    
    distances[i] = dist(plane, tr.get_vertice(i));
  }
  return distances;
}

ZeroSearch find_zero(const std::array<int, 3>& signs) noexcept
{
  for (int i = 0; i < 3; ++i)
  {
    if (signs[i] == 0)
      return {true, i};
  }
  return {false, 0};
}

MHIResult MollerHainsInterval(const Triangle3D& tr, const LineInf3D& line, const Plane3D& plane) noexcept
{
  std::array<double, 3> projections = get_projections(tr, line);
  std::array<double, 3> distances   = get_distances(tr, plane);
  std::array<int, 3>    dist_signs  = get_dist_signs(distances);

  int isep = find_separate_point(dist_signs); 
  if (isep == -1)
  {
    ZeroSearch zero = find_zero(dist_signs);
    if (zero.found)
    {
      double val = projections[zero.ind];
      return {MHIResult::SINGLE, {val, val}};
    }
    return {MHIResult::NONE}; // empty at this moment
  }

  MHIResult intersections;

  for (int i = 0; i < 2; ++i)
  {
    int ind = (isep + 1 + i) % 3;
    double di = dist(plane, tr.get_vertice(ind));
    double ds = dist(plane, tr.get_vertice(isep));
    double c = di / (di - ds);
    double t = projections[ind] + (projections[isep] - projections[ind]) * c;
    intersections.val[i] = t;
  }
  
  std::sort(intersections.val.begin(), intersections.val.end());
  return intersections;
}


// Reference: [GCT page 265]
bool intersect_complanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2)
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
bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2)
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();

  PlaneRelation pi = get_plane_relation(p1, p2);
  LineInf3D pil = get_plane_intersection(p1, p2, pi);

  MHIResult mhi1 = MollerHainsInterval(tr1, pil, p2);
  std::array<double, 2> intrs1 = mhi1.val;
  
  MHIResult mhi2 = MollerHainsInterval(tr2, pil, p1);
  std::array<double, 2> intrs2 = mhi2.val;

  if (mhi1.status == mhi1.NONE || mhi2.status == mhi2.NONE)
    return false;

  double max1 = intrs1.back();
  double min1 = intrs1.front();

  double max2 = intrs2.back();
  double min2 = intrs2.front();

  // Next we shold apply interval overlap method
  return intervals_intersect(min1, max1, min2, max2);
}

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2)
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();

  //std::cout << "Planes: ";
  //std::cout << p1;
  //std::cout << " | " << p2;

  // Intersect triangles' planes
  PlaneRelation pi = get_plane_relation(p1, p2);

  switch(pi.get_state())
  {
    case pi.COINCIDENT:
    {
      //std::cout << ": COINCIDENT" << std::endl; 
      return intersect_complanar_triangles(tr1, tr2);
    }

    case pi.PARALLEL:
    {
      //std::cout << ": PARALLEL" << std::endl;
      return false;
    }
    
    case pi.INTERSECTING:
    {
      //std::cout << ": INTERSECTING" << std::endl;
      return intersect_noncomplanar_triangles(tr1, tr2);
    }
  } 

  return false;
}


}