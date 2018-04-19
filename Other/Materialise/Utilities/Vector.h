#pragma once

#include <iostream>

struct Vector
{
	Vector();
	Vector(float x, float y, float z);
	~Vector() = default;

	float mX, mY, mZ;
};

bool AreEqual(const Vector& vector1, const Vector& vector2);
const float distance (const Vector& point1, const Vector& point2);
std::ostream& operator<< (std::ostream& os, const Vector& vec);
