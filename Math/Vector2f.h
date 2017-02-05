#pragma once

#include <iostream>
#include "Point2f.h"

struct Vector2f
{
    Vector2f();
    Vector2f(float x, float y);
    Vector2f(const Point2f& start, const Point2f& end);
    ~Vector2f() = default;

    float mX, mY;
};

const Vector2f operator+ (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator- (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator* (const Vector2f& vec, float scalar);
const Vector2f operator* (float scalar, const Vector2f& vec);
const Vector2f operator* (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator/ (const Vector2f& vec, float scalar);

const Vector2f normalize(const Vector2f& vec);
const Vector2f perp(const Vector2f& vec);

float length(const Vector2f& vec);
float dotProduct(const Vector2f& vec1, const Vector2f& vec2);

const Vector2f operator- (const Point2f& end, const Point2f start);
const Point2f operator+ (const Point2f& start, const Vector2f& vec);
const Point2f operator- (const Point2f& end, const Vector2f& vec);

std::ostream& operator <<(std::ostream& os, const Vector2f& vec);
