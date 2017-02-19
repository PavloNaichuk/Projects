#pragma once

#include <iostream>
#include "Point4f.h"

struct Vector4f
{
    Vector4f();
    Vector4f(float x, float y, float z, float w);
    Vector4f(const Point4f& start, const Point4f& end);
    ~Vector4f() = default;

    float mX, mY, mZ, mW;
};

const Vector4f operator+ (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator- (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator* (const Vector4f& vec, float scalar);
const Vector4f operator* (float scalar, const Vector4f& vec);
const Vector4f operator* (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator/ (const Vector4f& vec, float scalar);

const Vector4f normalize(const Vector4f& vec);

float length(const Vector4f& vec);
float dotProduct(const Vector4f& vec1, const Vector4f& vec2);

const Vector4f operator- (const Point4f& end, const Point4f start);
const Point4f operator+ (const Point4f& start, const Vector4f& vec);
const Point4f operator- (const Point4f& end, const Vector4f& vec);

std::ostream& operator<< (std::ostream& os, const Vector4f& vec);
