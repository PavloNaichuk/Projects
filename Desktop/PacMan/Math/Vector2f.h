#pragma once

#include "Math.h"

struct Vector2f
{
	explicit Vector2f();
	explicit Vector2f(float x, float y);
	explicit Vector2f(float scalar);

	float& operator[] (std::uint8_t index);
	const float& operator[] (std::uint8_t index) const;

	const Vector2f operator- () const;

	float m_X;
	float m_Y;

	static const Vector2f ONE;
	static const Vector2f ZERO;
	static const Vector2f UP;
	static const Vector2f DOWN;
	static const Vector2f LEFT;
	static const Vector2f RIGHT;
};

const Vector2f Abs(const Vector2f& vec);
const Vector2f Sqrt(const Vector2f& vec);
float Length(const Vector2f& vec);
float LengthSquared(const Vector2f& vec);
const Vector2f Normalize(const Vector2f& vec);
const Vector2f Rcp(const Vector2f& vec);
float Dot(const Vector2f& vec1, const Vector2f& vec2);
bool IsEqual(const Vector2f& vec1, const Vector2f& vec2, float epsilon = EPSILON);
const Vector2f Min(const Vector2f& vec1, const Vector2f& vec2);
const Vector2f Max(const Vector2f& vec1, const Vector2f& vec2);
bool IsNormalized(const Vector2f& vec, float epsilon = EPSILON);

Vector2f& operator+= (Vector2f& vec1, const Vector2f& vec2);
Vector2f& operator-= (Vector2f& vec1, const Vector2f& vec2);
Vector2f& operator*= (Vector2f& vec1, const Vector2f& vec2);
Vector2f& operator/= (Vector2f& vec1, const Vector2f& vec2);

Vector2f& operator+= (Vector2f& vec, float scalar);
Vector2f& operator-= (Vector2f& vec, float scalar);
Vector2f& operator*= (Vector2f& vec, float scalar);
Vector2f& operator/= (Vector2f& vec, float scalar);

const Vector2f operator+ (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator- (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator* (const Vector2f& vec1, const Vector2f& vec2);
const Vector2f operator/ (const Vector2f& vec1, const Vector2f& vec2);

const Vector2f operator+ (const Vector2f& vec, float scalar);
const Vector2f operator+ (float scalar, const Vector2f& vec);

const Vector2f operator- (const Vector2f& vec, float scalar);
const Vector2f operator- (float scalar, const Vector2f& vec);

const Vector2f operator* (const Vector2f& vec, float scalar);
const Vector2f operator* (float scalar, const Vector2f& vec);

const Vector2f operator/ (const Vector2f& vec, float scalar);
const Vector2f operator/ (float scalar, const Vector2f& vec);