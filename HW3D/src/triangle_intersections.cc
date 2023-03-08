/* Triangle-triangle intersection check implementation */

#include "triangle_intersections.hpp"

namespace HW3D
{

int sgn(data_t val) {
    return lesser(data_t(0), val) - lesser(val, data_t(0));
}

int distsign(const data_t dist) noexcept
{
  //return (fit(dist, 0)) ? 0 : 1 - (2 * std::signbit(dist));
  return sgn(dist);
}

int find_separate_point(const std::array<int, 3>& signs) noexcept
{
  for (int i = 0; i < 3; ++i)
  { 
    int s_i = signs[i];
    if (!s_i) continue;
    
    int s_i_r = signs[(i+1)%3];
    if(s_i == s_i_r) continue;

    int s_i_l = signs[(i+2)%3];
    if(s_i == s_i_l) continue;
    
    return i;
  }
  return -1;
}

std::array<int, 3> get_dist_signs(const std::array<data_t, 3>& distances) noexcept
{
  std::array<int, 3> signs;
  for (int i = 0; i < 3; ++i)
  {
    signs[i] = distsign(distances[i]);
  }
  return signs;
}

data_t get_projection(const Point3D& vertice, const LineInf3D& line) noexcept
{
  return line.get_a() * (vertice - line.get_p());
}

std::array<data_t, 3> get_projections(const Triangle3D& tr, const LineInf3D& line) noexcept
{
  std::array<data_t, 3> projections;
  for (int i = 0; i < 3; ++i)
  { 
    projections[i] = get_projection(tr.get_vertice(i), line);
  }
  return projections;
}

std::array<data_t, 3> get_distances(const Triangle3D& tr, const Plane3D& plane) noexcept
{
  std::array<data_t, 3> distances;
  for (int i = 0; i < 3; ++i)
  {    
    distances[i] = dist(plane, tr.get_vertice(i));
  }
  return distances;
}

ZeroSearch find_zero(const std::array<int, 3>& signs) noexcept
{
  for (size_t i = 0; i < 3; ++i)
  {
    if (signs[i] == 0)
      return {true, i};
  }
  return {false, 0};
}

MHIResult MollerHainsInterval(const Triangle3D& tr, const LineInf3D& line, const Plane3D& plane) noexcept
{
  std::array<data_t, 3> distances   = {dist(plane, tr.get_vertice(0)),
                                       dist(plane, tr.get_vertice(1)),
                                       dist(plane, tr.get_vertice(2))};
  
  std::array<int, 3>    dist_signs  = {distsign(distances[0]),
                                       distsign(distances[1]),
                                       distsign(distances[2])};

  int isep = find_separate_point(dist_signs); 
  if (isep == -1)
  {
    ZeroSearch zero = find_zero(dist_signs);
    if (zero.found)
    {
      data_t val = get_projection(tr.get_vertice(zero.ind), line);
      return {MHIResult::SINGLE, {val, val}};
    }
    return {MHIResult::NONE};
  }

  MHIResult intersections {MHIResult::FOUND};
  
  std::array<data_t, 3> projections = {get_projection(tr.get_vertice(0), line),
                                       get_projection(tr.get_vertice(1), line),
                                       get_projection(tr.get_vertice(2), line)};

  for (int i = 0; i < 2; ++i)
  {
    int ind = (isep + 1 + i) % 3;
    data_t di = distances[ind];
    data_t ds = distances[isep];
    data_t c = di / (di - ds);
    data_t t = projections[ind] + (projections[isep] - projections[ind]) * c;
    intersections.val[i] = t;
  }
  
  std::sort(intersections.val.begin(), intersections.val.end());
  return intersections;
}


bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2)
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();

  PlaneRelation pi = get_plane_relation(p1, p2);
  return intersect_noncomplanar_triangles(tr1, tr2, p1, p2, pi);
}

bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2)
{
  PlaneRelation pi = get_plane_relation(p1, p2);
  return intersect_noncomplanar_triangles(tr1, tr2, p1, p2, pi);
}

// Reference: [GCT page 541]
bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2, const PlaneRelation& pi)
{
  LineInf3D pil = get_plane_intersection(p1, p2, pi);
  
  MHIResult mhi1 = MollerHainsInterval(tr1, pil, p2);
  if (mhi1.status == mhi1.NONE) return false;
  std::array<data_t, 2>& intrs1 = mhi1.val;
  
  MHIResult mhi2 = MollerHainsInterval(tr2, pil, p1);
  if (mhi2.status == mhi2.NONE) return false;
  std::array<data_t, 2>& intrs2 = mhi2.val;

  data_t max1 = intrs1.back();
  data_t min1 = intrs1.front();

  data_t max2 = intrs2.back();
  data_t min2 = intrs2.front();

  // Next we shold apply interval overlap method
  return intervals_intersect(min1, max1, min2, max2);
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

    data_t max1 = interval1[1];
    data_t min1 = interval1[0];

    data_t max2 = interval2[1];
    data_t min2 = interval2[0];

    if (!intervals_intersect(min1, max1, min2, max2))
      return false;
  }

  for (int i = 0; i < 3; i++)
  {
    auto interval1 = ComputeInterval(tr1, normals2[i]);
    auto interval2 = ComputeInterval(tr2, normals2[i]);

    data_t max1 = interval1[1];
    data_t min1 = interval1[0];

    data_t max2 = interval2[1];
    data_t min2 = interval2[0];

    if (!intervals_intersect(min1, max1, min2, max2))
      return false;
  }

  return true;
}

std::array<data_t,2> ComputeInterval(const Triangle3D& tr, const Vec3D& direction)
{
  assert(direction != (Vec3D{0, 0, 0}));

  std::vector<data_t> projections;

  for (int i = 0; i < 3; i++)
  {
    Point3D v = tr.get_vertice(i);
    data_t t = v * direction; 
    projections.push_back(t);
  }

  auto minmax = std::minmax_element(projections.begin(), projections.end());

  return {*minmax.first, *minmax.second}; 
}

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2)
{
  Plane3D p1 = tr1.get_plane();
  Plane3D p2 = tr2.get_plane();
  return intersect_triangles(tr1, tr2, p1, p2);
}

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2)
{
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
      return intersect_noncomplanar_triangles(tr1, tr2, p1, p2, pi);
    }
  } 

  return false;
}


}