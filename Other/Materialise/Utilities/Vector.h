#pragma once

#include <string>

struct Vector
{
	Vector();
	Vector(float x, float y, float z);
	~Vector() = default;

	float mX, mY, mZ;
};

bool areEqual(const Vector& vec1, const Vector& vec2);
float distance(const Vector& point1, const Vector& point2);

std::string toString(const Vector& vec);
std::wstring toWString(const Vector& vec);

template <typename Stream>
Stream& operator<< (Stream& os, const Vector& vec)
{
	os << "(" << vec.mX << " , " << vec.mY << " , " << vec.mZ << ")";
	return os;
}