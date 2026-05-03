#pragma once

#include "Point2f.h"
#include "Vector2f.h"

struct Line;

struct Ray2f
{
    Ray2f(const Point2f& origin, const Vector2f& dir);
    ~Ray2f() = default;

    Point2f mOrigin;
    Vector2f mDir;
};

const Point2f calcPointAtParam(const Ray2f& ray, float param);
bool Overlaps(const Ray2f& ray, const Line& line, Point2f* pOverlapPoint = nullptr);
