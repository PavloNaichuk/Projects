#include "Vector.h"

Vector::Vector()
	: Vector(0.0f, 0.0f)
{
}

Vector::Vector(float x, float y)
	: mX(x)
	, mY(y)
{
}

const Vector operator+ (const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.mX + vec2.mX, vec1.mY + vec2.mY);
}

const Vector operator- (const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.mX - vec2.mX, vec1.mY - vec2.mY);
}

const Vector operator* (const Vector& vec, float scalar)
{
	return Vector(vec.mX * scalar, vec.mY * scalar);
}

const Vector operator* (float scalar, const Vector& vec)
{
	return Vector(scalar * vec.mX, scalar * vec.mY);
}

const Vector operator* (const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.mX * vec2.mX, vec1.mY * vec2.mY);
}

const Vector operator/(const Vector & vec, float scalar)
{
	return (1.0f / scalar) * vec;
}

const Vector& operator+= (Vector& vec1, const Vector& vec2)
{
	vec1.mX += vec2.mX;
	vec1.mY += vec2.mY;
	return vec1;
}

const Vector& operator-= (Vector& vec1, const Vector& vec2)
{
	vec1.mX -= vec2.mX;
	vec1.mY -= vec2.mY;
	return vec1;
}

const Vector& operator*= (Vector& vec1, const Vector& vec2)
{
	vec1.mX *= vec2.mX;
	vec1.mY *= vec2.mY;
	return vec1;
}

const Vector& operator*= (Vector& vec, float scalar)
{
	vec.mX *= scalar;
	vec.mY *= scalar;
	return vec;
}

std::ostream& operator<< (std::ostream& os, const Vector& vec)
{
	os << vec.mX << ", " << vec.mY;
	return os;
}