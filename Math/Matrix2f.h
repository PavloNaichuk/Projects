#pragma once

#include <iostream>

struct Matrix2f
{
    Matrix2f();
    Matrix2f(float e00, float e01, float e10, float e11);
    ~Matrix2f() = default;

    float m00, m01;
    float m10, m11;

    static const Matrix2f identity;
};

const Matrix2f operator+ (const Matrix2f& matrix1, const Matrix2f& matrix2);
const Matrix2f operator- (const Matrix2f& matrix1, const Matrix2f& matrix2);
const Matrix2f operator* (const Matrix2f& matrix1, const Matrix2f& matrix2);
const Matrix2f operator* (const Matrix2f& matrix, float scalar);
const Matrix2f operator* (float scalar, const Matrix2f& matrix);

float determinant(const Matrix2f& matrix);
const Matrix2f transpose(const Matrix2f& matrix);
bool inverse(const Matrix2f& matrix, Matrix2f* pInvMatrix);

std::ostream& operator<< (std::ostream& os, const Matrix2f& matrix);
