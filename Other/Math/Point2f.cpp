#include "Point2f.h"

Point2f::Point2f()
    : Point2f(0.0f, 0.0f)
{
}

Point2f::Point2f(float x, float y)
    : mX(x)
    , mY(y)
{
}

float distance(const Point2f& point1, const Point2f& point2)
{
    float diffX = point2.mX - point1.mX;
    float diffY = point2.mY - point1.mY;

    return std::sqrtf(diffX * diffX + diffY * diffY);
}

std::ostream& operator<< (std::ostream& os, const Point2f& point)
{
    os << point.mX << ", " << point.mY;
    return os;
}
