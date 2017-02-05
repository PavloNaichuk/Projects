#pragma once

#include <iostream>

struct Point2f
{
    Point2f();
    Point2f(float x, float y);
    ~Point2f() = default;

    float mX, mY;
};

float distance(const Point2f& point1, const Point2f& point2);
std::ostream& operator<< (std::ostream& os, const Point2f& point);
