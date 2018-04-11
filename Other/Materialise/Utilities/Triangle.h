#pragma once
#include <iostream>
#include "Vector.h"

struct Triangle
{
	Triangle() = default;
	Triangle(const Vector& normal, const Vector& vertex1, const Vector& vertex2, const Vector& vertex3);
	~Triangle() = default;

	Vector mNormal;
	Vector mVertex1;
	Vector mVertex2;
	Vector mVertex3;
};
bool AreEqual(const Triangle& triangle1, const Triangle& triangle2);
std::ostream& operator<< (std::ostream& os, const Triangle& triangle);

