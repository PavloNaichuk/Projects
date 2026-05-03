#include "Vector3f.h"

const Vector3f Vector3f::ONE(1.0f, 1.0f, 1.0f);
const Vector3f Vector3f::ZERO(0.0f, 0.0f, 0.0f);
const Vector3f Vector3f::UP(0.0f, 1.0f, 0.0f);
const Vector3f Vector3f::DOWN(0.0f, -1.0f, 0.0f);
const Vector3f Vector3f::LEFT(-1.0f, 0.0f, 0.0f);
const Vector3f Vector3f::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3f Vector3f::BACK(0.0f, 0.0f, -1.0f);
const Vector3f Vector3f::FORWARD(0.0f, 0.0f, 1.0f);

Vector3f::Vector3f()
    : Vector3f(0.0f)
{
}

Vector3f::Vector3f(float x, float y, float z)
    : m_X(x)
    , m_Y(y)
    , m_Z(z)
{
}

Vector3f::Vector3f(float scalar)
    : Vector3f(scalar, scalar, scalar)
{
}

float& Vector3f::operator[] (std::uint8_t index)
{
	assert(index < 3);
	return *(&m_X + index);
}

const float& Vector3f::operator[] (std::uint8_t index) const
{
	assert(index < 3);
	return *(&m_X + index);
}

const Vector3f Vector3f::operator- () const
{
	return Vector3f(-m_X, -m_Y, -m_Z);
}

const Vector3f Abs(const Vector3f& vec)
{
    return Vector3f(std::abs(vec.m_X), std::abs(vec.m_Y), std::abs(vec.m_Z));
}

const Vector3f Sqrt(const Vector3f& vec)
{
    return Vector3f(std::sqrt(vec.m_X), std::sqrt(vec.m_Y), std::sqrt(vec.m_Z));
}

float Length(const Vector3f& vec)
{
    return std::sqrt(Dot(vec, vec));
}

float LengthSquared(const Vector3f& vec)
{
    return Dot(vec, vec);
}

const Vector3f Normalize(const Vector3f& vec)
{
    return ((1.0f / Length(vec)) * vec);
}

const Vector3f Rcp(const Vector3f& vec)
{
    return Vector3f(1.0f / vec.m_X, 1.0f / vec.m_Y, 1.0f / vec.m_Z);
}

float Dot(const Vector3f& vec1, const Vector3f& vec2)
{
    return (vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y + vec1.m_Z * vec2.m_Z);
}

const Vector3f Cross(const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(vec1.m_Y * vec2.m_Z - vec1.m_Z * vec2.m_Y,
        vec1.m_Z * vec2.m_X - vec1.m_X * vec2.m_Z,
        vec1.m_X * vec2.m_Y - vec1.m_Y * vec2.m_X);
}

bool IsEqual(const Vector3f& vec1, const Vector3f& vec2, float epsilon)
{
    return (IsEqual(vec1.m_X, vec2.m_X, epsilon) && 
        IsEqual(vec1.m_Y, vec2.m_Y, epsilon) &&
        IsEqual(vec1.m_Z, vec2.m_Z, epsilon));
}

const Vector3f Min(const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(std::min(vec1.m_X, vec2.m_X), std::min(vec1.m_Y, vec2.m_Y), std::min(vec1.m_Z, vec2.m_Z));
}

const Vector3f Max(const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(std::max(vec1.m_X, vec2.m_X), std::max(vec1.m_Y, vec2.m_Y), std::max(vec1.m_Z, vec2.m_Z));
}

bool IsNormalized(const Vector3f& vec, float epsilon)
{
	return IsEqual(1.0f, Length(vec), epsilon);
}

Vector3f& operator+= (Vector3f& vec1, const Vector3f& vec2)
{
	vec1.m_X += vec2.m_X;
	vec1.m_Y += vec2.m_Y;
	vec1.m_Z += vec2.m_Z;
    return vec1;
}

Vector3f& operator-= (Vector3f& vec1, const Vector3f& vec2)
{
	vec1.m_X -= vec2.m_X;
	vec1.m_Y -= vec2.m_Y;
	vec1.m_Z -= vec2.m_Z;
    return vec1;
}

Vector3f& operator*= (Vector3f& vec1, const Vector3f& vec2)
{
	vec1.m_X *= vec2.m_X;
	vec1.m_Y *= vec2.m_Y;
	vec1.m_Z *= vec2.m_Z;
    return vec1;
}

Vector3f& operator/= (Vector3f& vec1, const Vector3f& vec2)
{
	vec1.m_X /= vec2.m_X;
	vec1.m_Y /= vec2.m_Y;
	vec1.m_Z /= vec2.m_Z;
    return vec1;
}

Vector3f& operator+= (Vector3f& vec, float scalar)
{
	vec.m_X += scalar;
	vec.m_Y += scalar;
	vec.m_Z += scalar;
    return vec;
}

Vector3f& operator-= (Vector3f& vec, float scalar)
{
	vec.m_X -= scalar;
	vec.m_Y -= scalar;
	vec.m_Z -= scalar;
    return vec;
}

Vector3f& operator*= (Vector3f& vec, float scalar)
{
	vec.m_X *= scalar;
	vec.m_Y *= scalar;
	vec.m_Z *= scalar;
    return vec;
}

Vector3f& operator/= (Vector3f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
	vec.m_X *= rcpScalar;
	vec.m_Y *= rcpScalar;
	vec.m_Z *= rcpScalar;
    return vec;
}

const Vector3f operator+ (const Vector3f& vec1, const Vector3f& vec2)
{
    return Vector3f(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y, vec1.m_Z + vec2.m_Z);
}

const Vector3f operator- (const Vector3f& vec1, const Vector3f& vec2)
{
	return Vector3f(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y, vec1.m_Z - vec2.m_Z);
}

const Vector3f operator* (const Vector3f& vec1, const Vector3f& vec2)
{
	return Vector3f(vec1.m_X * vec2.m_X, vec1.m_Y * vec2.m_Y, vec1.m_Z * vec2.m_Z);
}

const Vector3f operator/ (const Vector3f& vec1, const Vector3f& vec2)
{
	return Vector3f(vec1.m_X / vec2.m_X, vec1.m_Y / vec2.m_Y, vec1.m_Z / vec2.m_Z);
}

const Vector3f operator+ (const Vector3f& vec, float scalar)
{
    return Vector3f(vec.m_X + scalar, vec.m_Y + scalar, vec.m_Z + scalar);
}

const Vector3f operator+ (float scalar, const Vector3f& vec)
{
	return Vector3f(scalar + vec.m_X, scalar + vec.m_Y, scalar + vec.m_Z);
}

const Vector3f operator- (const Vector3f& vec, float scalar)
{
    return Vector3f(vec.m_X - scalar, vec.m_Y - scalar, vec.m_Z - scalar);
}

const Vector3f operator- (float scalar, const Vector3f& vec)
{
	return Vector3f(scalar - vec.m_X, scalar - vec.m_Y, scalar - vec.m_Z);
}

const Vector3f operator* (const Vector3f& vec, float scalar)
{
    return Vector3f(vec.m_X * scalar, vec.m_Y * scalar, vec.m_Z * scalar);
}

const Vector3f operator* (float scalar, const Vector3f& vec)
{
	return Vector3f(scalar * vec.m_X, scalar * vec.m_Y, scalar * vec.m_Z);
}

const Vector3f operator/ (const Vector3f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
    return Vector3f(vec.m_X * rcpScalar, vec.m_Y * rcpScalar, vec.m_Z * rcpScalar);
}

const Vector3f operator/ (float scalar, const Vector3f& vec)
{
	return Vector3f(scalar / vec.m_X, scalar / vec.m_Y, scalar / vec.m_Z);
}
