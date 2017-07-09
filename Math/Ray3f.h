#pragma once

#include "Point3f.h"
#include "Vector3f.h"

struct Plane;

struct Ray3f
{
    Ray3f(const Point3f& origin, const Vector3f& dir);
    ~Ray3f() = default;

    Point3f mOrigin;
    Vector3f mDir;
};

const Point3f calcPointAtParam(const Ray3f& ray, float param);
bool Overlaps(const Ray3f& ray, const Plane& plane, Point3f* pOverlapPoint = nullptr);
