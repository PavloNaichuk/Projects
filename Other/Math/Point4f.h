#pragma once

#include <iostream>

struct Point4f
{
    Point4f();
    Point4f(float x, float y, float z, float w);
    ~Point4f() = default;

    float mX, mY, mZ, mW;
};

float distance(const Point4f& point1, const Point4f& point2);
std::ostream& operator<< (std::ostream& os, const Point4f& point);
