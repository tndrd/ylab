#pragma once

#include "intersection.hpp"
#include <cassert>
#include <list>

namespace HW3D
{

struct PointsEntry
{
  std::vector<Point3D> pts;
  int n;
};

Triangle3D read_triangle(std::istream& stream);

std::list<PointsEntry> read_triangles(std::istream& stream);

std::vector<int> count_intersections(std::list<PointsEntry>& triangles);

void write_intersections(std::ostream& stream, const std::vector<int>& intersections);

void task_e2e(std::istream& in, std::ostream& out);
}