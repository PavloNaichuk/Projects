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

std::ostream& operator<< (std::ostream& os, const Vector& vec)
{
	os << vec.mX << " , " << vec.mY << " , " << vec.mZ << "\n";
	return os;
}
