/* Contains all the intersection check algorithms for each pair of primitives (except triangle-triangle) */

#pragma once

#include "Vec3D.hpp"
#include "lines.hpp"
#include "Triangle3D.hpp"
#include "triangle_intersections.hpp"

namespace HW3D
{

bool base_intersects(const Point3D& pt1, const Point3D& pt2);
bool base_intersects(const Point3D& pt, const LineSeg3D& l);
bool base_intersects(const Point3D& pt, const PlaneTriangle& plt);
bool base_intersects(const LineSeg3D& l, const Point3D& pt);
bool base_intersects(const LineSeg3D& l1, const LineSeg3D& l2);
bool base_intersects(const LineSeg3D& l, const PlaneTriangle& plt);
bool base_intersects(const PlaneTriangle& plt, const Point3D& pt);
bool base_intersects(const PlaneTriangle& plt, const LineSeg3D& l);
bool base_intersects(const PlaneTriangle& plt1, const PlaneTriangle& plt2);

}