#pragma once

#include <iostream>

struct Vector
{
	Vector();
	Vector(float x, float y);
	~Vector() = default;

	float mX, mY;
};
typedef Vector Point;
typedef Vector Size;

const Vector operator+ (const Vector& vec1, const Vector& vec2);
const Vector operator- (const Vector& vec1, const Vector& vec2);
const Vector operator* (const Vector& vec, float scalar);
const Vector operator* (float scalar, const Vector& vec);
const Vector operator* (const Vector& vec1, const Vector& vec2);
const Vector operator/ (const Vector& vec, float scalar);

const Vector& operator+= (Vector& vec1, const Vector& vec2);
const Vector& operator-= (Vector& vec1, const Vector& vec2);
const Vector& operator*= (Vector& vec1, const Vector& vec2);
const Vector& operator*= (Vector& vec, float scalar);

std::ostream& operator <<(std::ostream& os, const Vector& vec);

Vector Reflect(const Vector& incidentRay, const Vector& normal);
