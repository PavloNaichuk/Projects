#include "Vector4f.h"

Vector4f::Vector4f()
    : Vector4f(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Vector4f::Vector4f(float x, float y, float z, float w)
    : mX(x)
    , mY(y)
    , mZ(z)
    , mW(w)
{
}
Vector4f::Vector4f(const Point4f& start, const Point4f& end)
    : Vector4f(end.mX - start.mX, end.mY - start.mY, end.mZ - start.mZ, end.mW - start.mW)
{
}

const Vector4f operator+ (const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(vec1.mX + vec2.mX, vec1.mY + vec2.mY, vec1.mZ + vec2.mZ, vec1.mW + vec2.mW);
}

const Vector4f operator- (const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(vec1.mX - vec2.mX, vec1.mY - vec2.mY, vec1.mZ - vec2.mZ, vec1.mW - vec2.mW);
}

const Vector4f operator* (const Vector4f& vec, float scalar)
{
    return Vector4f(vec.mX * scalar, vec.mY * scalar, vec.mZ * scalar, vec.mW * scalar);
}

const Vector4f operator* (float scalar, const Vector4f& vec)
{
    return Vector4f(scalar * vec.mX, scalar * vec.mY, scalar * vec.mZ, scalar * vec.mW);
}

const Vector4f operator* (const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(vec1.mX * vec2.mX, vec1.mY * vec2.mY, vec1.mZ * vec2.mZ, vec1.mW * vec2.mW);
}

const Vector4f operator/ (const Vector4f& vec, float scalar)
{
    return (1.0f / scalar) * vec;
}

float length(const Vector4f& vec)
{
    return std::sqrtf(dotProduct(vec, vec));
}

const Vector4f normalize(const Vector4f& vec)
{
    return vec * (1.0f / length(vec));
}

float dotProduct(const Vector4f& vec1, const Vector4f& vec2)
{
    return (vec1.mX * vec2.mX + vec1.mY * vec2.mY + vec1.mZ * vec2.mZ + vec1.mW * vec2.mW);
}

const Vector4f operator- (const Point4f& end, const Point4f start)
{
    return Vector4f(start, end);
}

const Point4f operator+ (const Point4f& start, const Vector4f& vec)
{
    return Point4f(start.mX + vec.mX , start.mY + vec.mY, start.mZ + vec.mZ, start.mW + vec.mW);
}
const Point4f operator- (const Point4f& end, const Vector4f& vec)
{
    return Point4f(end.mX - vec.mX , end.mY - vec.mY, end.mZ - vec.mZ, end.mW - vec.mW);
}

std::ostream& operator<< (std::ostream& os, const Vector4f& vec)
{
    os << vec.mX << " , " << vec.mY << " , " << vec.mZ << " , " << vec.mW;
    return os;
}
