#pragma once

#include "Triangle3D.hpp"
#include "primitive_intersections.hpp"
#include <cassert>

namespace HW3D
{
std::array<data_t,2> ComputeInterval(const Triangle3D& tr, const Vec3D& direction);

bool intersect_complanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2);

bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2, const PlaneRelation& pi);
bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2);
bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2);

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2);
bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2, const Plane3D& p1, const Plane3D& p2);

struct MHIResult
{
  enum {
    FOUND,
    NONE,
    SINGLE,
  } status;
  std::array<data_t, 2> val = {0, 0};
};

struct ZeroSearch
{
  bool found;
  size_t ind;
};

MHIResult MollerHainsInterval(const Triangle3D& tr, const LineInf3D& line, const Plane3D& plane) noexcept;
int distsign(data_t dist) noexcept;
int find_separate_point(const std::array<int, 3>& signs) noexcept;
std::array<int, 3> get_dist_signs(const std::array<data_t, 3>& distances) noexcept;
std::array<data_t, 3> get_projections(const Triangle3D& tr, const LineInf3D& line) noexcept;
std::array<data_t, 3> get_distances(const Triangle3D& tr, const Plane3D& plane) noexcept;
ZeroSearch find_zero(const std::array<int, 3>& signs) noexcept;

}