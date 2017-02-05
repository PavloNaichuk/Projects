#include "Point3f.h"

Point3f::Point3f()
    : Point3f(0.0f, 0.0f, 0.0f)
{
}

Point3f::Point3f(float x, float y, float z)
    : mX(x)
    , mY(y)
    , mZ(z)
{
}

float distance(const Point3f& point1, const Point3f& point2)
{
    float diffX = point2.mX - point1.mX;
    float diffY = point2.mY - point1.mY;
    float diffZ = point2.mZ - point1.mZ;

    return std::sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

std::ostream& operator<< (std::ostream& os, const Point3f& point)
{
    os << point.mX << " , " << point.mY << " , " << point.mZ;
    return os;
}

