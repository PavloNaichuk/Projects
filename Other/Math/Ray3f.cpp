#include "Ray3f.h"
#include "Utilities.h"
#include "Plane.h"

Ray3f::Ray3f(const Point3f& origin, const Vector3f& dir)
    : mOrigin(origin)
    , mDir(dir)
{}

const Point3f calcPointAtParam(const Ray3f& ray, float param)
{
    return (ray.mOrigin + param * ray.mDir);
}

bool Overlaps(const Ray3f& ray, const Plane& plane, Point3f* pOverlapPoint)
{
    float denom = dotProduct(plane.mNormal, ray.mDir);

    if (equals(denom, 0.0f))
        return false;

    float hitParam = dotProduct(plane.mNormal, plane.mPoint - ray.mOrigin) / denom;
    if (pOverlapPoint != nullptr)
        *pOverlapPoint = calcPointAtParam(ray, hitParam);
    return true;
}
