#pragma once
#include <iostream>

struct Vector
{
	Vector();
	Vector(float x, float y, float z);
	~Vector() = default;

	float mX, mY, mZ;
};

std::ostream& operator<< (std::ostream& os, const Vector& vec);
