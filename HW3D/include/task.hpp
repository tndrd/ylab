#pragma once

#include "intersection.hpp"

namespace HW3D
{
Triangle3D read_triangle(std::istream& stream);

std::vector<Triangle3D> read_triangles(std::istream& stream);

std::vector<int> count_intersections(const std::vector<Triangle3D>& triangles);

void write_intersections(std::ostream& stream, const std::vector<int>& intersections);

void task_e2e(std::istream& in, std::ostream& out);
}