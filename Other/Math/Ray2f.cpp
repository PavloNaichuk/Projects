#include "Ray2f.h"
#include "Utilities.h"
#include "Line.h"

Ray2f::Ray2f(const Point2f& origin, const Vector2f& dir)
    : mOrigin(origin)
    , mDir(dir)
{}

const Point2f calcPointAtParam(const Ray2f& ray, float param)
{
    return (ray.mOrigin + param * ray.mDir);
}

bool Overlaps(const Ray2f& ray, const Line& line, Point2f* pOverlapPoint)
{
    Vector2f normal = perp(line.mDir);
    float denom = dotProduct(normal, ray.mDir);

    if (equals(denom, 0.0f))
        return false;

    float hitParam = dotProduct(normal, line.mStart - ray.mOrigin) / denom;
    if (pOverlapPoint != nullptr)
        *pOverlapPoint = calcPointAtParam(ray, hitParam);
    return true;
}
