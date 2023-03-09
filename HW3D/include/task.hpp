/* Contains HW3D lvl1-specific functionality */

#pragma once

#include "intersectible.hpp"
#include <cassert>
#include <list>

namespace HW3D
{

struct PointsEntry
{
  std::unique_ptr<IIntersectible> object;
  size_t n;
  bool in = false;
};

Point3D read_point(std::istream& stream);
std::unique_ptr<IIntersectible> read_object(std::istream& stream);

std::vector<PointsEntry> read_objects(std::istream& stream);

std::vector<int> count_intersections(std::vector<PointsEntry>& triangles);

void write_intersections(std::ostream& stream, const std::vector<int>& intersections);

void task_e2e(std::istream& in, std::ostream& out);
}