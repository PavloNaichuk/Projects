#include "Vector4f.h"

const Vector4f Vector4f::ONE(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4f Vector4f::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

Vector4f::Vector4f()
    : Vector4f(0.0f)
{
}

Vector4f::Vector4f(float x, float y, float z, float w)
    : m_X(x)
    , m_Y(y)
    , m_Z(z)
    , m_W(w)
{
}

Vector4f::Vector4f(float scalar)
    : Vector4f(scalar, scalar, scalar, scalar)
{
}

float& Vector4f::operator[] (std::uint8_t index)
{
	assert(index < 4);
	return *(&m_X + index);
}

const float& Vector4f::operator[] (std::uint8_t index) const
{
	assert(index < 4);
	return *(&m_X + index);
}

const Vector4f Vector4f::operator- () const
{
	return Vector4f(-m_X, -m_Y, -m_Z, -m_W);
}

const Vector4f Abs(const Vector4f& vec)
{
    return Vector4f(std::abs(vec.m_X), std::abs(vec.m_Y), std::abs(vec.m_Z), std::abs(vec.m_W));
}

const Vector4f Sqrt(const Vector4f& vec)
{
    return Vector4f(std::sqrt(vec.m_X), std::sqrt(vec.m_Y), std::sqrt(vec.m_Z), std::sqrt(vec.m_W));
}

float Length(const Vector4f& vec)
{
    return std::sqrt(Dot(vec, vec));
}

float LengthSquared(const Vector4f& vec)
{
    return Dot(vec, vec);
}

const Vector4f Normalize(const Vector4f& vec)
{
    return ((1.0f / Length(vec)) * vec);
}

const Vector4f Rcp(const Vector4f& vec)
{
    return Vector4f(1.0f / vec.m_X, 1.0f / vec.m_Y, 1.0f / vec.m_Z, 1.0f / vec.m_W);
}

float Dot(const Vector4f& vec1, const Vector4f& vec2)
{
    return (vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y + vec1.m_Z * vec2.m_Z + vec1.m_W * vec2.m_W);
}

bool IsEqual(const Vector4f& vec1, const Vector4f& vec2, float epsilon)
{
    return (IsEqual(vec1.m_X, vec2.m_X, epsilon) &&
        IsEqual(vec1.m_Y, vec2.m_Y, epsilon) &&
        IsEqual(vec1.m_Z, vec2.m_Z, epsilon) &&
        IsEqual(vec1.m_W, vec2.m_W, epsilon));
}

const Vector4f Min(const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(std::min(vec1.m_X, vec2.m_X), std::min(vec1.m_Y, vec2.m_Y), std::min(vec1.m_Z, vec2.m_Z), std::min(vec1.m_W, vec2.m_W));
}

const Vector4f Max(const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(std::max(vec1.m_X, vec2.m_X), std::max(vec1.m_Y, vec2.m_Y), std::max(vec1.m_Z, vec2.m_Z), std::max(vec1.m_W, vec2.m_W));
}

bool IsNormalized(const Vector4f& vec, float epsilon)
{
	return IsEqual(1.0f, Length(vec), epsilon);
}

Vector4f& operator+= (Vector4f& vec1, const Vector4f& vec2)
{
	vec1.m_X += vec2.m_X;
	vec1.m_Y += vec2.m_Y;
	vec1.m_Z += vec2.m_Z;
	vec1.m_W += vec2.m_W;
    return vec1;
}

Vector4f& operator-= (Vector4f& vec1, const Vector4f& vec2)
{
	vec1.m_X -= vec2.m_X;
	vec1.m_Y -= vec2.m_Y;
	vec1.m_Z -= vec2.m_Z;
	vec1.m_W -= vec2.m_W;
    return vec1;
}

Vector4f& operator*= (Vector4f& vec1, const Vector4f& vec2)
{
	vec1.m_X *= vec2.m_X;
	vec1.m_Y *= vec2.m_Y;
	vec1.m_Z *= vec2.m_Z;
	vec1.m_W *= vec2.m_W;
    return vec1;
}

Vector4f& operator/= (Vector4f& vec1, const Vector4f& vec2)
{
	vec1.m_X /= vec2.m_X;
	vec1.m_Y /= vec2.m_Y;
	vec1.m_Z /= vec2.m_Z;
	vec1.m_W /= vec2.m_W;
    return vec1;
}

Vector4f& operator+= (Vector4f& vec, float scalar)
{
	vec.m_X += scalar;
	vec.m_Y += scalar;
	vec.m_Z += scalar;
	vec.m_W += scalar;
    return vec;
}

Vector4f& operator-= (Vector4f& vec, float scalar)
{
	vec.m_X -= scalar;
	vec.m_Y -= scalar;
	vec.m_Z -= scalar;
	vec.m_W -= scalar;
    return vec;
}

Vector4f& operator*= (Vector4f& vec, float scalar)
{
	vec.m_X *= scalar;
	vec.m_Y *= scalar;
	vec.m_Z *= scalar;
	vec.m_W *= scalar;
    return vec;
}

Vector4f& operator/= (Vector4f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
	vec.m_X *= rcpScalar;
	vec.m_Y *= rcpScalar;
	vec.m_Z *= rcpScalar;
	vec.m_W *= rcpScalar;
    return vec;
}

const Vector4f operator+ (const Vector4f& vec1, const Vector4f& vec2)
{
    return Vector4f(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y, vec1.m_Z + vec2.m_Z, vec1.m_W + vec2.m_W);
}

const Vector4f operator- (const Vector4f& vec1, const Vector4f& vec2)
{
	return Vector4f(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y, vec1.m_Z - vec2.m_Z, vec1.m_W - vec2.m_W);
}

const Vector4f operator* (const Vector4f& vec1, const Vector4f& vec2)
{
	return Vector4f(vec1.m_X * vec2.m_X, vec1.m_Y * vec2.m_Y, vec1.m_Z * vec2.m_Z, vec1.m_W * vec2.m_W);
}

const Vector4f operator/ (const Vector4f& vec1, const Vector4f& vec2)
{
	return Vector4f(vec1.m_X / vec2.m_X, vec1.m_Y / vec2.m_Y, vec1.m_Z / vec2.m_Z, vec1.m_W / vec2.m_W);
}

const Vector4f operator+ (const Vector4f& vec, float scalar)
{
    return Vector4f(vec.m_X + scalar, vec.m_Y + scalar, vec.m_Z + scalar, vec.m_W + scalar);
}

const Vector4f operator+ (float scalar, const Vector4f& vec)
{
	return Vector4f(scalar + vec.m_X, scalar + vec.m_Y, scalar + vec.m_Z, scalar + vec.m_W);
}

const Vector4f operator- (const Vector4f& vec, float scalar)
{
    return Vector4f(vec.m_X - scalar, vec.m_Y - scalar, vec.m_Z - scalar, vec.m_W - scalar);
}

const Vector4f operator- (float scalar, const Vector4f& vec)
{
	return Vector4f(scalar - vec.m_X, scalar - vec.m_Y, scalar - vec.m_Z, scalar - vec.m_W);
}

const Vector4f operator* (const Vector4f& vec, float scalar)
{
	return Vector4f(vec.m_X * scalar, vec.m_Y * scalar, vec.m_Z * scalar, vec.m_W * scalar);
}

const Vector4f operator* (float scalar, const Vector4f& vec)
{
	return Vector4f(scalar * vec.m_X, scalar * vec.m_Y, scalar * vec.m_Z, scalar * vec.m_W);
}

const Vector4f operator/ (const Vector4f& vec, float scalar)
{
	float rcpScalar = 1.0f / scalar;
    return Vector4f(vec.m_X * rcpScalar, vec.m_Y * rcpScalar, vec.m_Z * rcpScalar, vec.m_W * rcpScalar);
}

const Vector4f operator/ (float scalar, const Vector4f& vec)
{
	return Vector4f(scalar / vec.m_X, scalar / vec.m_Y, scalar / vec.m_Z, scalar / vec.m_W);
}
