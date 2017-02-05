#include "Vector3f.h"

Vector3f::Vector3f()
    : Vector3f(0.0f, 0.0f, 0.0f)
{
}

Vector3f::Vector3f(float x, float y, float z)
    : mX(x)
    , mY(y)
    , mZ(z)
{
}
Vector3f::Vector3f(const Point3f& start, const Point3f& end)
    :Vector3f(end.mX - start.mX, end.mY - start.mY, end.mZ - start.mZ)
{
}

const Vector3f operator+ (const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(vec1.mX + vec2.mX, vec1.mY + vec2.mY, vec1.mZ + vec2.mZ);
}

const Vector3f operator- (const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(vec1.mX - vec2.mX, vec1.mY - vec2.mY, vec1.mZ - vec2.mZ);
}

const Vector3f operator* (const Vector3f& vec, float scalar)
{
    return Vector3f(vec.mX * scalar, vec.mY * scalar, vec.mZ * scalar);
}

const Vector3f operator* (float scalar, const Vector3f& vec)
{
    return Vector3f(scalar * vec.mX, scalar * vec.mY, scalar * vec.mZ);
}

const Vector3f operator* (const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(vec1.mX * vec2.mX, vec1.mY * vec2.mY, vec1.mZ * vec2.mZ);
}

const Vector3f operator/ (const Vector3f& vec, float scalar)
{
    return (1.0f / scalar) * vec;
}

float length(const Vector3f& vec)
{
    return std::sqrtf(dotProduct(vec, vec));
}

const Vector3f normalize(const Vector3f& vec)
{
    return vec * (1.0f / length(vec));
}

float dotProduct(const Vector3f& vec1, const Vector3f& vec2)
{
    return (vec1.mX * vec2.mX + vec1.mY * vec2.mY + vec1.mZ * vec2.mZ);
}

const Vector3f crossProduct(const Vector3f& vec1, const Vector3f& vec2)
{
   return Vector3f(vec1.mY * vec2.mZ - vec1.mZ * vec2.mY,
           vec1.mZ * vec2.mX - vec1.mX * vec2.mZ,
           vec1.mX * vec2.mY - vec1.mY * vec2.mX);
}

const Vector3f operator- (const Point3f& end, const Point3f start)
{
    return Vector3f(start, end);
}

const Point3f operator+ (const Point3f& start, const Vector3f& vec)
{
    return Point3f(start.mX + vec.mX , start.mY + vec.mY, start.mZ + vec.mZ);
}
const Point3f operator- (const Point3f& end, const Vector3f& vec)
{
    return Point3f(end.mX - vec.mX , end.mY - vec.mY, end.mZ - vec.mZ);
}

std::ostream& operator<< (std::ostream& os, const Vector3f& vec)
{
    os << vec.mX << " , " << vec.mY << " , " << vec.mZ;
    return os;
}
