#pragma once

#include "Point3f.h"
#include "Vector3f.h"

struct Plane
{
    Plane(const Point3f& point, const Vector3f& normal);
    ~Plane() = default;

    Point3f mPoint;
    Vector3f mNormal;
};


