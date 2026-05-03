#pragma once

#include "Math.h"

struct Vector4f
{
	explicit Vector4f();
	explicit Vector4f(float x, float y, float z, float w);
    explicit Vector4f(float scalar);
    
	float& operator[] (std::uint8_t index);
	const float& operator[] (std::uint8_t index) const;

	const Vector4f operator- () const;

    float m_X;
    float m_Y;
    float m_Z;
    float m_W;

	static const Vector4f ONE;
	static const Vector4f ZERO;
};

const Vector4f Abs(const Vector4f& vec);
const Vector4f Sqrt(const Vector4f& vec);
float Length(const Vector4f& vec);
float LengthSquared(const Vector4f& vec);
const Vector4f Normalize(const Vector4f& vec);
const Vector4f Rcp(const Vector4f& vec);
float Dot(const Vector4f& vec1, const Vector4f& vec2);
bool IsEqual(const Vector4f& vec1, const Vector4f& vec2, float epsilon = EPSILON);
const Vector4f Min(const Vector4f& vec1, const Vector4f& vec2);
const Vector4f Max(const Vector4f& vec1, const Vector4f& vec2);
bool IsNormalized(const Vector4f& vec, float epsilon = EPSILON);

Vector4f& operator+= (Vector4f& vec1, const Vector4f& vec2);
Vector4f& operator-= (Vector4f& vec1, const Vector4f& vec2);
Vector4f& operator*= (Vector4f& vec1, const Vector4f& vec2);
Vector4f& operator/= (Vector4f& vec1, const Vector4f& vec2);

Vector4f& operator+= (Vector4f& vec, float scalar);
Vector4f& operator-= (Vector4f& vec, float scalar);
Vector4f& operator*= (Vector4f& vec, float scalar);
Vector4f& operator/= (Vector4f& vec, float scalar);

const Vector4f operator+ (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator- (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator* (const Vector4f& vec1, const Vector4f& vec2);
const Vector4f operator/ (const Vector4f& vec1, const Vector4f& vec2);

const Vector4f operator+ (const Vector4f& vec, float scalar);
const Vector4f operator+ (float scalar, const Vector4f& vec);

const Vector4f operator- (const Vector4f& vec, float scalar);
const Vector4f operator- (float scalar, const Vector4f& vec);

const Vector4f operator* (const Vector4f& vec, float scalar);
const Vector4f operator* (float scalar, const Vector4f& vec);

const Vector4f operator/ (const Vector4f& vec, float scalar);
const Vector4f operator/ (float scalar, const Vector4f& vec);
