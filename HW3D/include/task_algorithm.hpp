#pragma once

#include "Triangle3D.hpp"
#include "intersections.hpp"

namespace HW3D
{
std::array<double,2> ComputeInterval(const Triangle3D& tr, const Vec3D& direction);

bool intersect_complanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept;

bool intersect_noncomplanar_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept;

bool intersect_triangles(const Triangle3D& tr1, const Triangle3D& tr2) noexcept;
}