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

std::ostream& operator<< (std::ostream& os, const Vector& vec)
{
	os << vec.mX << " , " << vec.mY << " , " << vec.mZ << "\n";
	return os;
}
