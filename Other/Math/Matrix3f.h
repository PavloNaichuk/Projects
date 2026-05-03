#pragma once

#include <iostream>

struct Matrix3f
{
    Matrix3f();
    Matrix3f(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22);
    ~Matrix3f() = default;

    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;

    static const Matrix3f identity;
};

const Matrix3f operator+ (const Matrix3f& matrix1, const Matrix3f& matrix2);
const Matrix3f operator- (const Matrix3f& matrix1, const Matrix3f& matrix2);
const Matrix3f operator* (const Matrix3f& matrix1, const Matrix3f& matrix2);
const Matrix3f operator* (const Matrix3f& matrix, float scalar);
const Matrix3f operator* (float scalar, const Matrix3f& matrix);

float determinant(const Matrix3f& matrix);
const Matrix3f transpose(const Matrix3f& matrix);
bool inverse(const Matrix3f& matrix, Matrix3f* pInvMatrix);

std::ostream& operator<< (std::ostream& os, const Matrix3f& matrix);

