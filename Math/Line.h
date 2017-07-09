#pragma once

#include "Point2f.h"
#include "Vector2f.h"

struct Line
{
    Line(const Point2f& start, const Vector2f& dir);
    Line(const Point2f& start, const Point2f& end);
    ~Line() = default;

    Point2f mStart;
    Vector2f mDir;
};

const Point2f calcPointAtParam(const Line& line, float param);
bool Overlaps(const Line& line1, const Line& line2, Point2f* pOverlapPoint = nullptr);
