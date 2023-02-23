#pragma once

#include "intersection.hpp"
#include <cassert>
#include <list>

namespace HW3D
{

struct PointsEntry
{
  PointGroup pgroup;
  size_t n;
};

Point3D read_point(std::istream& stream);
Triangle3D read_triangle(std::istream& stream);

std::list<PointsEntry> read_triangles(std::istream& stream);

std::vector<int> count_intersections(std::list<PointsEntry>& triangles);

void write_intersections(std::ostream& stream, const std::vector<int>& intersections);

void task_e2e(std::istream& in, std::ostream& out);
}