#pragma once

#include <iostream>

struct Point3f
{
    Point3f();
    Point3f(float x, float y, float z);
    ~Point3f() = default;

    float mX, mY, mZ;
};

float distance(const Point3f& point1, const Point3f& point2);
std::ostream& operator<< (std::ostream& os, const Point3f& point);
