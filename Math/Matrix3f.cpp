#include "Matrix3f.h"
#include "Utilities.h"

#define determinant2x2(e00, e01, e10, e11) (e00 * e11 - e01 * e10)

Matrix3f::Matrix3f()
    : Matrix3f(0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f)
{
}

Matrix3f::Matrix3f(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22)
    : m00(e00)
    , m01(e01)
    , m02(e02)
    , m10(e10)
    , m11(e11)
    , m12(e12)
    , m20(e20)
    , m21(e21)
    , m22(e22)
{
}

const Matrix3f Matrix3f::identity(1.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f);

const Matrix3f operator+ (const Matrix3f& matrix1, const Matrix3f& matrix2)
{
    return Matrix3f(matrix1.m00 + matrix2.m00,
                    matrix1.m01 + matrix2.m01,
                    matrix1.m02 + matrix2.m02,
                    matrix1.m10 + matrix2.m10,
                    matrix1.m11 + matrix2.m11,
                    matrix1.m12 + matrix2.m12,
                    matrix1.m20 + matrix2.m20,
                    matrix1.m21 + matrix2.m21,
                    matrix1.m22 + matrix2.m22);
}

const Matrix3f operator- (const Matrix3f& matrix1, const Matrix3f& matrix2)
{
    return Matrix3f(matrix1.m00 - matrix2.m00,
                    matrix1.m01 - matrix2.m01,
                    matrix1.m02 - matrix2.m02,
                    matrix1.m10 - matrix2.m10,
                    matrix1.m11 - matrix2.m11,
                    matrix1.m12 - matrix2.m12,
                    matrix1.m20 - matrix2.m20,
                    matrix1.m21 - matrix2.m21,
                    matrix1.m22 - matrix2.m22);
}

const Matrix3f operator* (const Matrix3f& matrix1, const Matrix3f& matrix2)
{
    return Matrix3f(matrix1.m00 * matrix2.m00 + matrix1.m01 * matrix2.m10 + matrix1.m02 * matrix2.m20,
                    matrix1.m00 * matrix2.m01 + matrix1.m01 * matrix2.m11 + matrix1.m02 * matrix2.m21,
                    matrix1.m00 * matrix2.m02 + matrix1.m01 * matrix2.m12 + matrix1.m02 * matrix2.m22,
                    matrix1.m10 * matrix2.m00 + matrix1.m11 * matrix2.m10 + matrix1.m12 * matrix2.m20,
                    matrix1.m10 * matrix2.m01 + matrix1.m11 * matrix2.m11 + matrix1.m12 * matrix2.m21,
                    matrix1.m10 * matrix2.m02 + matrix1.m11 * matrix2.m12 + matrix1.m12 * matrix2.m22,
                    matrix1.m20 * matrix2.m00 + matrix1.m21 * matrix2.m10 + matrix1.m22 * matrix2.m20,
                    matrix1.m20 * matrix2.m01 + matrix1.m21 * matrix2.m11 + matrix1.m22 * matrix2.m21,
                    matrix1.m20 * matrix2.m02 + matrix1.m21 * matrix2.m12 + matrix1.m22 * matrix2.m22);
}

const Matrix3f operator* (const Matrix3f& matrix, float scalar)
{
   return Matrix3f(matrix.m00 * scalar, matrix.m01 * scalar, matrix.m02 * scalar,
                   matrix.m10 * scalar, matrix.m11 * scalar, matrix.m12 * scalar,
                   matrix.m20 * scalar, matrix.m21 * scalar, matrix.m22 * scalar);
}

const Matrix3f operator* (float scalar, const Matrix3f& matrix)
{
    return Matrix3f(scalar * matrix.m00, scalar * matrix.m01, scalar * matrix.m02,
                    scalar * matrix.m10, scalar * matrix.m11, scalar * matrix.m12,
                    scalar * matrix.m20, scalar * matrix.m21, scalar * matrix.m22);
}

float determinant(const Matrix3f& matrix)
{
    return (matrix.m00 * matrix.m11 * matrix.m22 +
            matrix.m01 * matrix.m12 * matrix.m20 +
            matrix.m02 * matrix.m10 * matrix.m21 -
            matrix.m02 * matrix.m11 * matrix.m20 -
            matrix.m00 * matrix.m12 * matrix.m21 -
            matrix.m01 * matrix.m10 * matrix.m22);
}

const Matrix3f transpose(const Matrix3f& matrix)
{
    return Matrix3f(matrix.m00, matrix.m10, matrix.m20,
                    matrix.m01, matrix.m11, matrix.m21,
                    matrix.m02, matrix.m12, matrix.m22);
}

bool inverse(const Matrix3f& matrix, Matrix3f* pInvMatrix)
{
    float det = determinant(matrix);
    if (equals(det, 0.0f))
        return false;

    float invDet = 1.0f / det;
    pInvMatrix->m00 = determinant2x2(matrix.m11, matrix.m12, matrix.m21, matrix.m22) * invDet;
    pInvMatrix->m01 = determinant2x2(matrix.m02, matrix.m01, matrix.m22, matrix.m21) * invDet;
    pInvMatrix->m02 = determinant2x2(matrix.m01, matrix.m02, matrix.m11, matrix.m12) * invDet;

    pInvMatrix->m10 = determinant2x2(matrix.m12, matrix.m10, matrix.m22, matrix.m20) * invDet;
    pInvMatrix->m11 = determinant2x2(matrix.m00, matrix.m02, matrix.m21, matrix.m22) * invDet;
    pInvMatrix->m12 = determinant2x2(matrix.m02, matrix.m00, matrix.m12, matrix.m10) * invDet;

    pInvMatrix->m20 = determinant2x2(matrix.m10, matrix.m11, matrix.m20, matrix.m21) * invDet;
    pInvMatrix->m21 = determinant2x2(matrix.m01, matrix.m00, matrix.m21, matrix.m20) * invDet;
    pInvMatrix->m22 = determinant2x2(matrix.m00, matrix.m01, matrix.m10, matrix.m11) * invDet;

    return true;
}

std::ostream& operator<< (std::ostream& os, const Matrix3f& matrix)
{
    os << matrix.m00 << "  " << matrix.m01 << " " <<  matrix.m02 << "\n";
    os << matrix.m10 << "  " << matrix.m11 << " " <<  matrix.m12 << "\n";
    os << matrix.m20 << "  " << matrix.m21 << " " <<  matrix.m22 << "\n";
    return os;
}
