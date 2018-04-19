#include "stdafx.h"
#include "Vector.h"

Vector::Vector()
	: Vector(0.0f, 0.0f, 0.0f)
{
}

Vector::Vector(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{
}

bool AreEqual(const Vector & vector1, const Vector & vector2)
{
	return ((vector1.mX == vector2.mX) && (vector1.mY == vector2.mY) && (vector1.mZ == vector2.mZ));
}

const float distance(const Vector& point1, const Vector& point2)
{
	float diffX = point2.mX - point1.mX;
	float diffY = point2.mY - point1.mY;
	float diffZ = point2.mZ - point1.mZ;

	return std::sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

std::ostream& operator<< (std::ostream& os, const Vector& vec)
{
	os << vec.mX << " , " << vec.mY << " , " << vec.mZ << "\n";
	return os;
}
