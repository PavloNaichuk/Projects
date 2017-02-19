#include "Point4f.h"

Point4f::Point4f()
    : Point4f(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Point4f::Point4f(float x, float y, float z, float w)
    : mX(x)
    , mY(y)
    , mZ(z)
    , mW(w)
{
}

float distance(const Point4f& point1, const Point4f& point2)
{
    float diffX = point2.mX - point1.mX;
    float diffY = point2.mY - point1.mY;
    float diffZ = point2.mZ - point1.mZ;
    float diffW = point2.mW - point1.mW;

    return std::sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ + diffW * diffW);
}

std::ostream& operator<< (std::ostream& os, const Point4f& point)
{
    os << point.mX << " , " << point.mY << " , " << point.mZ << " , " << point.mW;
    return os;
}
