#pragma once

#include <iostream>

struct Matrix4f
{
    Matrix4f();
    Matrix4f(float e00, float e01, float e02, float e03, float e10, float e11, float e12, float e13, float e20, float e21, float e22, float e23, float e30, float e31, float e32, float e33);
    ~Matrix4f() = default;

    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    static const Matrix4f identity;
};

const Matrix4f operator+ (const Matrix4f& matrix1, const Matrix4f& matrix2);
const Matrix4f operator- (const Matrix4f& matrix1, const Matrix4f& matrix2);
const Matrix4f operator* (const Matrix4f& matrix1, const Matrix4f& matrix2);
const Matrix4f operator* (const Matrix4f& matrix, float scalar);
const Matrix4f operator* (float scalar, const Matrix4f& matrix);

float determinant(const Matrix4f& matrix);
const Matrix4f transpose(const Matrix4f& matrix);
bool inverse(const Matrix4f& matrix, Matrix4f* pInvMatrix);
const Matrix4f Adjoint(const Matrix4f& matrix);
const Matrix4f Inverse(const Matrix4f& matrix);

std::ostream& operator<< (std::ostream& os, const Matrix4f& matrix);
