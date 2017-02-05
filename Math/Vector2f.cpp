#include "Vector2f.h"

Vector2f::Vector2f()
    : Vector2f(0.0f, 0.0f)
{
}

Vector2f::Vector2f(float x, float y)
    : mX(x)
    , mY(y)
{
}

Vector2f::Vector2f(const Point2f& start, const Point2f& end)
    : Vector2f(end.mX - start.mX, end.mY - start.mY)
{
}

const Vector2f operator+ (const Vector2f& vec1, const Vector2f& vec2)
{
    return Vector2f(vec1.mX + vec2.mX, vec1.mY + vec2.mY);
}

const Vector2f operator- (const Vector2f& vec1, const Vector2f& vec2)
{
    return Vector2f(vec1.mX - vec2.mX, vec1.mY - vec2.mY);
}

const Vector2f operator* (const Vector2f& vec, float scalar)
{
    return Vector2f(vec.mX * scalar, vec.mY * scalar);
}

const Vector2f operator* (float scalar, const Vector2f& vec)
{
    return Vector2f(scalar * vec.mX, scalar * vec.mY);
}

const Vector2f operator* (const Vector2f& vec1, const Vector2f& vec2)
{
    return Vector2f(vec1.mX * vec2.mX, vec1.mY * vec2.mY);
}

const Vector2f operator/ (const Vector2f& vec, float scalar)
{
    return (1.0f / scalar) * vec;
}

float length(const Vector2f& vec)
{
    return std::sqrtf(dotProduct(vec, vec));
}

const Vector2f normalize(const Vector2f& vec)
{
    return vec * (1.0f / length(vec));
}

float dotProduct(const Vector2f& vec1, const Vector2f& vec2)
{
    return (vec1.mX * vec2.mX + vec1.mY * vec2.mY);
}

const Vector2f perp(const Vector2f& vec)
{
    return Vector2f(-vec.mY, vec.mX);
}

const Vector2f operator- (const Point2f& end, const Point2f start)
{
    return Vector2f(start, end);
}

const Point2f operator+ (const Point2f& start, const Vector2f& vec)
{
    return Point2f(start.mX + vec.mX , start.mY + vec.mY);
}

const Point2f operator- (const Point2f& end, const Vector2f& vec)
{
    return Point2f(end.mX - vec.mX , end.mY - vec.mY);
}



std::ostream& operator<< (std::ostream& os, const Vector2f& vec)
{
    os << vec.mX << ", " << vec.mY;
    return os;
}
