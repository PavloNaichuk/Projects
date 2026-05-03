#pragma once

#include <iostream>
#include "Point3f.h"

struct Vector3f
{
    Vector3f();
    Vector3f(float x, float y, float z);
    Vector3f(const Point3f& start, const Point3f& end);
    ~Vector3f() = default;

    float mX, mY, mZ;
};

const Vector3f operator+ (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator- (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator* (const Vector3f& vec, float scalar);
const Vector3f operator* (float scalar, const Vector3f& vec);
const Vector3f operator* (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator/ (const Vector3f& vec, float scalar);

const Vector3f normalize(const Vector3f& vec);

float length(const Vector3f& vec);
float dotProduct(const Vector3f& vec1, const Vector3f& vec2);
const Vector3f crossProduct(const Vector3f& vec1, const Vector3f& vec2);

const Vector3f operator- (const Point3f& end, const Point3f start);
const Point3f operator+ (const Point3f& start, const Vector3f& vec);
const Point3f operator- (const Point3f& end, const Vector3f& vec);

std::ostream& operator<< (std::ostream& os, const Vector3f& vec);
