#pragma once

#include "Math.h"

struct Vector3f
{
	explicit Vector3f();
	explicit Vector3f(float x, float y, float z);
    explicit Vector3f(float scalar);

	float& operator[] (std::uint8_t index);
	const float& operator[] (std::uint8_t index) const;

	const Vector3f operator- () const;

    float m_X;
    float m_Y;
    float m_Z;

	static const Vector3f ONE;
	static const Vector3f ZERO;
	static const Vector3f UP;
	static const Vector3f DOWN;
	static const Vector3f LEFT;
	static const Vector3f RIGHT;
	static const Vector3f BACK;
	static const Vector3f FORWARD;
};

const Vector3f Abs(const Vector3f& vec);
const Vector3f Sqrt(const Vector3f& vec);
float Length(const Vector3f& vec);
float LengthSquared(const Vector3f& vec);
const Vector3f Normalize(const Vector3f& vec);
const Vector3f Rcp(const Vector3f& vec);
float Dot(const Vector3f& vec1, const Vector3f& vec2);
const Vector3f Cross(const Vector3f& vec1, const Vector3f& vec2);
bool IsEqual(const Vector3f& vec1, const Vector3f& vec2, float epsilon = EPSILON);
const Vector3f Min(const Vector3f& vec1, const Vector3f& vec2);
const Vector3f Max(const Vector3f& vec1, const Vector3f& vec2);
bool IsNormalized(const Vector3f& vec, float epsilon = EPSILON);

Vector3f& operator+= (Vector3f& vec1, const Vector3f& vec2);
Vector3f& operator-= (Vector3f& vec1, const Vector3f& vec2);
Vector3f& operator*= (Vector3f& vec1, const Vector3f& vec2);
Vector3f& operator/= (Vector3f& vec1, const Vector3f& vec2);

Vector3f& operator+= (Vector3f& vec, float scalar);
Vector3f& operator-= (Vector3f& vec, float scalar);
Vector3f& operator*= (Vector3f& vec, float scalar);
Vector3f& operator/= (Vector3f& vec, float scalar);

const Vector3f operator+ (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator- (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator* (const Vector3f& vec1, const Vector3f& vec2);
const Vector3f operator/ (const Vector3f& vec1, const Vector3f& vec2);

const Vector3f operator+ (const Vector3f& vec, float scalar);
const Vector3f operator+ (float scalar, const Vector3f& vec);

const Vector3f operator- (const Vector3f& vec, float scalar);
const Vector3f operator- (float scalar, const Vector3f& vec);

const Vector3f operator* (const Vector3f& vec, float scalar);
const Vector3f operator* (float scalar, const Vector3f& vec);

const Vector3f operator/ (const Vector3f& vec, float scalar);
const Vector3f operator/ (float scalar, const Vector3f& vec);
