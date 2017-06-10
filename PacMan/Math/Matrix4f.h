#pragma once

#include "Vector4f.h"

struct Matrix4f
{
    explicit Matrix4f();
    explicit Matrix4f(float scalar);
	explicit Matrix4f(float scalar00, float scalar01, float scalar02, float scalar03,
					  float scalar10, float scalar11, float scalar12, float scalar13,
					  float scalar20, float scalar21, float scalar22, float scalar23,
					  float scalar30, float scalar31, float scalar32, float scalar33);

	const Matrix4f operator- () const;
    
    float m_00, m_01, m_02, m_03;
    float m_10, m_11, m_12, m_13;
    float m_20, m_21, m_22, m_23;
    float m_30, m_31, m_32, m_33;

	static const Matrix4f IDENTITY;
	static const Matrix4f ZERO;
};

Matrix4f& operator+= (Matrix4f& matrix1, const Matrix4f& matrix2);
Matrix4f& operator-= (Matrix4f& matrix1, const Matrix4f& matrix2);
Matrix4f& operator*= (Matrix4f& matrix1, const Matrix4f& matrix2);

Matrix4f& operator+= (Matrix4f& matrix, float scalar);
Matrix4f& operator-= (Matrix4f& matrix, float scalar);
Matrix4f& operator*= (Matrix4f& matrix, float scalar);

const Matrix4f operator+ (const Matrix4f& matrix1, const Matrix4f& matrix2);
const Matrix4f operator- (const Matrix4f& matrix1, const Matrix4f& matrix2);
const Matrix4f operator* (const Matrix4f& matrix1, const Matrix4f& matrix2);

const Matrix4f operator+ (const Matrix4f& matrix, float scalar);
const Matrix4f operator+ (float scalar, const Matrix4f& matrix);

const Matrix4f operator- (const Matrix4f& matrix, float scalar);
const Matrix4f operator- (float scalar, const Matrix4f& matrix);

const Matrix4f operator* (const Matrix4f& matrix, float scalar);
const Matrix4f operator* (float scalar, const Matrix4f& matrix);

const Vector4f operator* (const Vector4f& vec, const Matrix4f& matrix);
const Vector4f operator* (const Matrix4f& matrix, const Vector4f& vec);

const Matrix4f Transpose(const Matrix4f& matrix);
float Determinant(const Matrix4f& matrix);
const Matrix4f Adjoint(const Matrix4f& matrix);
const Matrix4f Inverse(const Matrix4f& matrix);