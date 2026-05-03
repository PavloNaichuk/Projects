#include "Matrix2f.h"
#include "Utilities.h"

Matrix2f::Matrix2f()
    : Matrix2f(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Matrix2f::Matrix2f(float e00, float e01, float e10, float e11)
    : m00(e00)
    , m01(e01)
    , m10(e10)
    , m11(e11)
{
}

const Matrix2f Matrix2f::identity(1.0f, 0.0f, 0.0f, 1.0f);

const Matrix2f operator+ (const Matrix2f& matrix1, const Matrix2f& matrix2)
{
    return Matrix2f(matrix1.m00 + matrix2.m00,
                    matrix1.m01 + matrix2.m01,
                    matrix1.m10 + matrix2.m10,
                    matrix1.m11 + matrix2.m11);
}

const Matrix2f operator- (const Matrix2f& matrix1, const Matrix2f& matrix2)
{
    return Matrix2f(matrix1.m00 - matrix2.m00,
                    matrix1.m01 - matrix2.m01,
                    matrix1.m10 - matrix2.m10,
                    matrix1.m11 - matrix2.m11);
}

const Matrix2f operator* (const Matrix2f& matrix1, const Matrix2f& matrix2)
{
    return Matrix2f(matrix1.m00 * matrix2.m00 + matrix1.m01 * matrix2.m10,
                    matrix1.m00 * matrix2.m01 + matrix1.m01 * matrix2.m11,
                    matrix1.m10 * matrix2.m00 + matrix1.m11 * matrix2.m10,
                    matrix1.m10 * matrix2.m10 + matrix1.m11 * matrix2.m11);
}

const Matrix2f operator* (const Matrix2f& matrix, float scalar)
{
   return Matrix2f(matrix.m00 * scalar, matrix.m01 * scalar, matrix.m10 * scalar, matrix.m11 * scalar);
}

const Matrix2f operator* (float scalar, const Matrix2f& matrix)
{
    return Matrix2f(scalar * matrix.m00, scalar * matrix.m01, scalar * matrix.m10, scalar * matrix.m11);
}

float determinant(const Matrix2f& matrix)
{
    return (matrix.m00 * matrix.m11 - matrix.m01 * matrix.m10);
}

const Matrix2f transpose(const Matrix2f& matrix)
{
    return Matrix2f(matrix.m00, matrix.m10, matrix.m01, matrix.m11);
}

bool inverse(const Matrix2f& matrix, Matrix2f* pInvMatrix)
{
    float det = determinant(matrix);
    if (equals(det, 0.0f))
        return false;

    float invDet = 1.0f / det;
    pInvMatrix->m00 =  matrix.m00 * invDet;
    pInvMatrix->m01 = -matrix.m10 * invDet;
    pInvMatrix->m10 = -matrix.m01 * invDet;
    pInvMatrix->m11 =  matrix.m11 * invDet;

    return true;
}

std::ostream& operator<< (std::ostream& os, const Matrix2f& matrix)
{
    os << matrix.m00 << "  " << matrix.m01 << "\n";
    os << matrix.m10 << "  " << matrix.m11 << "\n";
    return os;
}
