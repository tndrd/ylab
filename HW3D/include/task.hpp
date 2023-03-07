#pragma once

#include "intersections.hpp"
#include <cassert>
#include <list>

namespace HW3D
{

struct PointsEntry
{
  std::unique_ptr<IntersectibleWrapper> object;
  size_t n;
};

Point3D read_point(std::istream& stream);
std::unique_ptr<IntersectibleWrapper> read_object(std::istream& stream);

std::list<PointsEntry> read_objects(std::istream& stream);

std::vector<int> count_intersections(std::list<PointsEntry>& triangles);

void write_intersections(std::ostream& stream, const std::vector<int>& intersections);

void task_e2e(std::istream& in, std::ostream& out);
}