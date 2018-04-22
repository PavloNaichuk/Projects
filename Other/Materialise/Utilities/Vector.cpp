#include "stdafx.h"
#include "Vector.h"
#include <sstream>

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

bool areEqual(const Vector& vec1, const Vector& vec2)
{
	return ((vec1.mX == vec2.mX) && (vec1.mY == vec2.mY) && (vec1.mZ == vec2.mZ));
}

float distance(const Vector& point1, const Vector& point2)
{
	float diffX = point2.mX - point1.mX;
	float diffY = point2.mY - point1.mY;
	float diffZ = point2.mZ - point1.mZ;

	return std::sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

std::string toString(const Vector& vec)
{
	std::stringstream stream;
	stream << vec;
	return stream.str();
}

std::wstring toWString(const Vector& vec)
{
	std::wstringstream stream;
	stream << vec;
	return stream.str();
}
