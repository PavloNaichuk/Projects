#include "Vector2f.h"

const Vector2f Vector2f::ONE(1.0f, 1.0f);
const Vector2f Vector2f::ZERO(0.0f, 0.0f);
const Vector2f Vector2f::UP(0.0f, 1.0f);
const Vector2f Vector2f::DOWN(0.0f, -1.0f);
const Vector2f Vector2f::LEFT(-1.0f, 0.0f);
const Vector2f Vector2f::RIGHT(1.0f, 0.0f);

Vector2f::Vector2f()
	: Vector2f(0.0f)
{
}

Vector2f::Vector2f(float x, float y)
	: m_X(x)
	, m_Y(y)
{
}

Vector2f::Vector2f(float scalar)
	: Vector2f(scalar, scalar)
{
}

float& Vector2f::operator[] (std::uint8_t index)
{
	assert(index < 2);
	return *(&m_X + index);
}

const float& Vector2f::operator[] (std::uint8_t index) const
{
	assert(index < 2);
	return *(&m_X + index);
}

const Vector2f Vector2f::operator- () const
{
	return Vector2f(-m_X, -m_Y);
}

const Vector2f Abs(const Vector2f& vec)
{
	return Vector2f(std::abs(vec.m_X), std::abs(vec.m_Y));
}

const Vector2f Sqrt(const Vector2f& vec)
{
	return Vector2f(std::sqrtf(vec.m_X), std::sqrtf(vec.m_Y));
}

float Length(const Vector2f& vec)
{
	return std::sqrt(Dot(vec, vec));
}

float LengthSquared(const Vector2f& vec)
{
	return Dot(vec, vec);
}

const Vector2f Normalize(const Vector2f& vec)
{
	return ((1.0f / Length(vec)) * vec);
}

const Vector2f Rcp(const Vector2f& vec)
{
	return Vector2f(1.0f / vec.m_X, 1.0f / vec.m_Y);
}

float Dot(const Vector2f& vec1, const Vector2f& vec2)
{
	return (vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y);
}

bool IsEqual(const Vector2f& vec1, const Vector2f& vec2, float epsilon)
{
	return (IsEqual(vec1.m_X, vec2.m_X, epsilon) && IsEqual(vec1.m_Y, vec2.m_Y, epsilon));
}

const Vector2f Min(const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(std::min(vec1.m_X, vec2.m_X), std::min(vec1.m_Y, vec2.m_Y));
}

const Vector2f Max(const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(std::max(vec1.m_X, vec2.m_X), std::max(vec1.m_Y, vec2.m_Y));
}

bool IsNormalized(const Vector2f& vec, float epsilon)
{
	return IsEqual(1.0f, Length(vec), epsilon);
}

Vector2f& operator+= (Vector2f& vec1, const Vector2f& vec2)
{
	vec1.m_X += vec2.m_X;
	vec1.m_Y += vec2.m_Y;
	return vec1;
}

Vector2f& operator-= (Vector2f& vec1, const Vector2f& vec2)
{
	vec1.m_X -= vec2.m_X;
	vec1.m_Y -= vec2.m_Y;
	return vec1;
}

Vector2f& operator*= (Vector2f& vec1, const Vector2f& vec2)
{
	vec1.m_X *= vec2.m_X;
	vec1.m_Y *= vec2.m_Y;
	return vec1;
}

Vector2f& operator/= (Vector2f& vec1, const Vector2f& vec2)
{
	vec1.m_X /= vec2.m_X;
	vec1.m_Y /= vec2.m_Y;
	return vec1;
}

Vector2f& operator+= (Vector2f& vec, float scalar)
{
	vec.m_X += scalar;
	vec.m_Y += scalar;
	return vec;
}

Vector2f& operator-= (Vector2f& vec, float scalar)
{
	vec.m_X -= scalar;
	vec.m_Y -= scalar;
	return vec;
}

Vector2f& operator*= (Vector2f& vec, float scalar)
{
	vec.m_X *= scalar;
	vec.m_Y *= scalar;
	return vec;
}

Vector2f& operator/= (Vector2f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
	vec.m_X *= rcpScalar;
	vec.m_Y *= rcpScalar;
	return vec;
}

const Vector2f operator+ (const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y);
}

const Vector2f operator- (const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y);
}

const Vector2f operator* (const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(vec1.m_X * vec2.m_X, vec1.m_Y * vec2.m_Y);
}

const Vector2f operator/ (const Vector2f& vec1, const Vector2f& vec2)
{
	return Vector2f(vec1.m_X / vec2.m_X, vec1.m_Y / vec2.m_Y);
}

const Vector2f operator+ (const Vector2f& vec, float scalar)
{
	return Vector2f(vec.m_X + scalar, vec.m_Y + scalar);
}

const Vector2f operator+ (float scalar, const Vector2f& vec)
{
	return Vector2f(scalar + vec.m_X, scalar + vec.m_Y);
}

const Vector2f operator- (const Vector2f& vec, float scalar)
{
	return Vector2f(vec.m_X - scalar, vec.m_Y - scalar);
}

const Vector2f operator- (float scalar, const Vector2f& vec)
{
	return Vector2f(scalar - vec.m_X, scalar - vec.m_Y);
}

const Vector2f operator* (const Vector2f& vec, float scalar)
{
	return Vector2f(vec.m_X * scalar, vec.m_Y * scalar);
}

const Vector2f operator* (float scalar, const Vector2f& vec)
{
	return Vector2f(scalar * vec.m_X, scalar * vec.m_Y);
}

const Vector2f operator/ (const Vector2f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
	return Vector2f(vec.m_X * rcpScalar, vec.m_Y * rcpScalar);
}

const Vector2f operator/ (float scalar, const Vector2f& vec)
{
	return Vector2f(scalar / vec.m_X, scalar / vec.m_Y);
}
