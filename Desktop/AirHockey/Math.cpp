#include "pch.h"
#include "Math.h"
#include "Common.h"

Vector::Vector()
	: Vector(0.0f, 0.0f)
{
}

Vector::Vector(float x, float y)
	: mX(x)
	, mY(y)
{
}

const Vector Vector::operator- () const
{
	return Vector(-mX, -mY);
}

const Vector operator+ (const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.mX + vec2.mX, vec1.mY + vec2.mY);
}

const Vector operator+ (float scalar, const Vector& vec) 
{
	return Vector(scalar + vec.mX, scalar + vec.mY);
}

const Vector operator+ (const Vector& vec, float scalar) 
{
	return Vector(scalar + vec.mX, scalar + vec.mY);
}

const Vector operator- (const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.mX - vec2.mX, vec1.mY - vec2.mY);
}

const Vector operator- (float scalar, const Vector& vec) 
{
	return Vector(scalar - vec.mX, scalar - vec.mY);
}

const Vector operator- (const Vector& vec, float scalar) 
{
	return Vector(vec.mX - scalar, vec.mY - scalar);
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

const Vector operator/(const Vector& vec, float scalar)
{
	return (1.0f / scalar) * vec;
}

const Vector operator/ (float scalar, const Vector& vec) 
{
	return Vector(scalar / vec.mX, scalar / vec.mY);
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

bool AreEqual(float value1, float value2)
{
	return (std::abs(value1 - value2) < 0.0001f);
}

const Vector Perp(const Vector& vec)
{
	return Vector(-vec.mY, vec.mX);
}

float Dot(const Vector& vec1, const Vector& vec2) 
{
	return (vec1.mX * vec2.mX + vec1.mY * vec2.mY);
}

const Vector Reflect(const Vector& incident, const Vector& normal)
{
	return (incident - (2.0f * Dot(incident, normal)) * normal);
}

const Vector Normalize(const Vector& vec)
{
	float rcpLength = 1.0f / Length(vec);
	return rcpLength * vec;
}

float Length(const Vector& vec)
{
	return std::sqrtf(Dot(vec, vec));
}

float SquaredLength(const Vector& vec)
{
	return Dot(vec, vec);
}

float Distance(const Point& point1, const Point& point2)
{
	return Length(point1 - point2);
}

float SquaredDistance(const Point& point1, const Point& point2)
{
	return SquaredLength(point1 - point2);
}

bool IsZero(const Vector& vec) 
{
	return (AreEqual(vec.mX, 0.0f) && AreEqual(vec.mY, 0.0f));
}

Ray::Ray(const Point& origin, const Vector& direction)
	: mOrigin(origin)
	, mDirection(direction)
{
}

const Point CalcPointOnRay(const Ray& ray, float param)
{
	return (ray.mOrigin + param * ray.mDirection);
}

bool TestHit(const Ray& ray1, const Ray& ray2, float& hitParam1, float& hitParam2)
{
	const Vector perpDir2 = Perp(ray2.mDirection);
	if (AreEqual(Dot(ray1.mDirection, perpDir2), 0.0f))
		return false;
		
	const Vector perpDir1 = Perp(ray1.mDirection);
	hitParam1 = Dot(ray2.mOrigin - ray1.mOrigin, perpDir2) / Dot(ray1.mDirection, perpDir2);
	hitParam2 = Dot(ray1.mOrigin - ray2.mOrigin, perpDir1) / Dot(ray2.mDirection, perpDir1);

	return true;
}

float Square(float value)
{
	return (value * value);
}

bool CirclesHitEachOther(const Point& center1, float radius1, const Point& center2, float radius2)
{
	return (SquaredDistance(center1, center2) <= Square(radius1 + radius2));
}

float Random(float from, float to) 
{
	float random = (float)std::rand() / (float)RAND_MAX;
	return (from + random * (to - from));
}