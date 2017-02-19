#include "Matrix4f.h"
#include "Utilities.h"

#define determinant3x3(e00, e01, e02, e10, e11, e12, e20, e21, e22) (e00 * e11 * e22 + e01 * e12 * e20 + e02 * e10 * e21 - e02 * e11 * e20 - e00 * e12 * e21 - e01 * e10 * e22)
#define DETERMINANT3X3(m00, m01, m02, m10, m11, m12, m20, m21, m22) \
    (m20 * (m01 * m12 - m02 * m11) + \
     m21 * (m02 * m10 - m00 * m12) + \
     m22 * (m00 * m11 - m01 * m10))

Matrix4f::Matrix4f()
    : Matrix4f(0.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 0.0f)
{
}

Matrix4f::Matrix4f(float e00, float e01, float e02, float e03, float e10, float e11, float e12, float e13, float e20, float e21, float e22, float e23, float e30, float e31, float e32, float e33)
    : m00(e00)
    , m01(e01)
    , m02(e02)
    , m03(e03)
    , m10(e10)
    , m11(e11)
    , m12(e12)
    , m13(e13)
    , m20(e20)
    , m21(e21)
    , m22(e22)
    , m23(e23)
    , m30(e30)
    , m31(e31)
    , m32(e32)
    , m33(e33)
{
}

const Matrix4f Matrix4f::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4f operator+ (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return Matrix4f(matrix1.m00 + matrix2.m00,
                    matrix1.m01 + matrix2.m01,
                    matrix1.m02 + matrix2.m02,
                    matrix1.m03 + matrix2.m03,
                    matrix1.m10 + matrix2.m10,
                    matrix1.m11 + matrix2.m11,
                    matrix1.m12 + matrix2.m12,
                    matrix1.m13 + matrix2.m13,
                    matrix1.m20 + matrix2.m20,
                    matrix1.m21 + matrix2.m21,
                    matrix1.m22 + matrix2.m22,
                    matrix1.m23 + matrix2.m23,
                    matrix1.m30 + matrix2.m30,
                    matrix1.m31 + matrix2.m31,
                    matrix1.m32 + matrix2.m32,
                    matrix1.m33 + matrix2.m33);
}

const Matrix4f operator- (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return Matrix4f(matrix1.m00 - matrix2.m00,
                    matrix1.m01 - matrix2.m01,
                    matrix1.m02 - matrix2.m02,
                    matrix1.m03 - matrix2.m03,
                    matrix1.m10 - matrix2.m10,
                    matrix1.m11 - matrix2.m11,
                    matrix1.m12 - matrix2.m12,
                    matrix1.m13 - matrix2.m13,
                    matrix1.m20 - matrix2.m20,
                    matrix1.m21 - matrix2.m21,
                    matrix1.m22 - matrix2.m22,
                    matrix1.m23 - matrix2.m23,
                    matrix1.m30 - matrix2.m30,
                    matrix1.m31 - matrix2.m31,
                    matrix1.m32 - matrix2.m32,
                    matrix1.m33 - matrix2.m33);
}

const Matrix4f operator* (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return Matrix4f(matrix1.m00 * matrix2.m00 + matrix1.m01 * matrix2.m10 + matrix1.m02 * matrix2.m20 + matrix1.m03 * matrix2.m30,
                    matrix1.m00 * matrix2.m01 + matrix1.m01 * matrix2.m11 + matrix1.m02 * matrix2.m21 + matrix1.m03 * matrix2.m31,
                    matrix1.m00 * matrix2.m02 + matrix1.m01 * matrix2.m12 + matrix1.m02 * matrix2.m22 + matrix1.m03 * matrix2.m32,
                    matrix1.m00 * matrix2.m03 + matrix1.m01 * matrix2.m13 + matrix1.m02 * matrix2.m23 + matrix1.m03 * matrix2.m33,
                    matrix1.m10 * matrix2.m00 + matrix1.m11 * matrix2.m10 + matrix1.m12 * matrix2.m20 + matrix1.m13 * matrix2.m30,
                    matrix1.m10 * matrix2.m01 + matrix1.m11 * matrix2.m11 + matrix1.m12 * matrix2.m21 + matrix1.m13 * matrix2.m31,
                    matrix1.m10 * matrix2.m02 + matrix1.m11 * matrix2.m12 + matrix1.m12 * matrix2.m22 + matrix1.m13 * matrix2.m32,
                    matrix1.m10 * matrix2.m03 + matrix1.m11 * matrix2.m13 + matrix1.m12 * matrix2.m23 + matrix1.m13 * matrix2.m33,
                    matrix1.m20 * matrix2.m00 + matrix1.m21 * matrix2.m10 + matrix1.m22 * matrix2.m20 + matrix1.m23 * matrix2.m30,
                    matrix1.m20 * matrix2.m01 + matrix1.m21 * matrix2.m11 + matrix1.m22 * matrix2.m21 + matrix1.m23 * matrix2.m31,
                    matrix1.m20 * matrix2.m02 + matrix1.m21 * matrix2.m12 + matrix1.m22 * matrix2.m22 + matrix1.m23 * matrix2.m32,
                    matrix1.m20 * matrix2.m03 + matrix1.m21 * matrix2.m13 + matrix1.m22 * matrix2.m23 + matrix1.m23 * matrix2.m33,
                    matrix1.m30 * matrix2.m00 + matrix1.m31 * matrix2.m10 + matrix1.m32 * matrix2.m20 + matrix1.m33 * matrix2.m30,
                    matrix1.m30 * matrix2.m01 + matrix1.m31 * matrix2.m11 + matrix1.m32 * matrix2.m21 + matrix1.m33 * matrix2.m31,
                    matrix1.m30 * matrix2.m02 + matrix1.m31 * matrix2.m12 + matrix1.m32 * matrix2.m22 + matrix1.m33 * matrix2.m32,
                    matrix1.m30 * matrix2.m03 + matrix1.m31 * matrix2.m13 + matrix1.m32 * matrix2.m23 + matrix1.m33 * matrix2.m33);
}

const Matrix4f operator* (const Matrix4f& matrix, float scalar)
{
   return Matrix4f(matrix.m00 * scalar, matrix.m01 * scalar, matrix.m02 * scalar, matrix.m03 * scalar,
                   matrix.m10 * scalar, matrix.m11 * scalar, matrix.m12 * scalar, matrix.m13 * scalar,
                   matrix.m20 * scalar, matrix.m21 * scalar, matrix.m22 * scalar, matrix.m23 * scalar,
                   matrix.m30 * scalar, matrix.m31 * scalar, matrix.m32 * scalar, matrix.m33 * scalar);
}

const Matrix4f operator* (float scalar, const Matrix4f& matrix)
{
    return Matrix4f(scalar * matrix.m00, scalar * matrix.m01, scalar * matrix.m02, scalar * matrix.m03,
                    scalar * matrix.m10, scalar * matrix.m11, scalar * matrix.m12, scalar * matrix.m13,
                    scalar * matrix.m20, scalar * matrix.m21, scalar * matrix.m22, scalar * matrix.m23,
                    scalar * matrix.m30, scalar * matrix.m31, scalar * matrix.m32, scalar * matrix.m33);
}

float determinant(const Matrix4f& matrix)
{
    return (
            matrix.m00 * determinant3x3(matrix.m11, matrix.m12, matrix.m13, matrix.m21, matrix.m22, matrix.m23, matrix.m31, matrix.m32, matrix.m33) -
            matrix.m01 * determinant3x3(matrix.m10, matrix.m12, matrix.m13, matrix.m20, matrix.m22, matrix.m23, matrix.m30, matrix.m32, matrix.m33) +
            matrix.m02 * determinant3x3(matrix.m10, matrix.m11, matrix.m13, matrix.m20, matrix.m21, matrix.m23, matrix.m30, matrix.m31, matrix.m33) -
            matrix.m03 * determinant3x3(matrix.m10, matrix.m11, matrix.m12, matrix.m20, matrix.m21, matrix.m22, matrix.m30, matrix.m31, matrix.m32)
            );
}

const Matrix4f transpose(const Matrix4f& matrix)
{
    return Matrix4f(matrix.m00, matrix.m10, matrix.m20, matrix.m30,
                    matrix.m01, matrix.m11, matrix.m21, matrix.m31,
                    matrix.m02, matrix.m12, matrix.m22, matrix.m32,
                    matrix.m03, matrix.m13, matrix.m23, matrix.m33);
}

bool inverse(const Matrix4f& matrix, Matrix4f* pInvMatrix)
{
   float det = determinant(matrix);
    if (equals(det, 0.0f))
        return false;

    float invDet = 1.0f / det;
    pInvMatrix->m00 = determinant3x3(matrix.m11, matrix.m12, matrix.m13, matrix.m21, matrix.m22, matrix.m23, matrix.m31, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m01 = -determinant3x3(matrix.m10, matrix.m12, matrix.m13, matrix.m20, matrix.m22, matrix.m23, matrix.m30, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m02 = determinant3x3(matrix.m10, matrix.m11, matrix.m13, matrix.m20, matrix.m21, matrix.m23, matrix.m30, matrix.m31, matrix.m33) * invDet;
    pInvMatrix->m03 = -determinant3x3(matrix.m10, matrix.m11, matrix.m12, matrix.m20, matrix.m21, matrix.m22, matrix.m30, matrix.m31, matrix.m32) * invDet;

    pInvMatrix->m10 = -determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m21, matrix.m22, matrix.m23, matrix.m31, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m11 = determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m20, matrix.m22, matrix.m23, matrix.m30, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m12 = -determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m20, matrix.m21, matrix.m23, matrix.m30, matrix.m31, matrix.m33) * invDet;
    pInvMatrix->m13 = determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m20, matrix.m21, matrix.m22, matrix.m30, matrix.m31, matrix.m32) * invDet;

    pInvMatrix->m20 = determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m11, matrix.m12, matrix.m13, matrix.m31, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m21 = -determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m10, matrix.m12, matrix.m13, matrix.m30, matrix.m32, matrix.m33) * invDet;
    pInvMatrix->m22 = determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m10, matrix.m11, matrix.m13, matrix.m30, matrix.m31, matrix.m33) * invDet;
    pInvMatrix->m23 = -determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m10, matrix.m11, matrix.m12, matrix.m30, matrix.m31, matrix.m32) * invDet;

    pInvMatrix->m30 = -determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m11, matrix.m12, matrix.m13, matrix.m21, matrix.m22, matrix.m23) * invDet;
    pInvMatrix->m31 = determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m10, matrix.m12, matrix.m13, matrix.m20, matrix.m22, matrix.m23) * invDet;
    pInvMatrix->m32 = -determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m10, matrix.m11, matrix.m13, matrix.m20, matrix.m21, matrix.m23) * invDet;
    pInvMatrix->m33 = determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m10, matrix.m11, matrix.m12, matrix.m20, matrix.m21, matrix.m22) * invDet;
    return true;
}

const Matrix4f Adjoint(const Matrix4f& matrix)
{
    float cofactor00 = DETERMINANT3X3(matrix.m11, matrix.m12, matrix.m13,
        matrix.m21, matrix.m22, matrix.m23,
        matrix.m31, matrix.m32, matrix.m33);

    float cofactor01 = -DETERMINANT3X3(matrix.m10, matrix.m12, matrix.m13,
        matrix.m20, matrix.m22, matrix.m23,
        matrix.m30, matrix.m32, matrix.m33);

    float cofactor02 = DETERMINANT3X3(matrix.m10, matrix.m11, matrix.m13,
        matrix.m20, matrix.m21, matrix.m23,
        matrix.m30, matrix.m31, matrix.m33);

    float cofactor03 = -DETERMINANT3X3(matrix.m10, matrix.m11, matrix.m12,
        matrix.m20, matrix.m21, matrix.m22,
        matrix.m30, matrix.m31, matrix.m32);

    float cofactor10 = -DETERMINANT3X3(matrix.m01, matrix.m02, matrix.m03,
        matrix.m21, matrix.m22, matrix.m23,
        matrix.m31, matrix.m32, matrix.m33);

    float cofactor11 = DETERMINANT3X3(matrix.m00, matrix.m02, matrix.m03,
        matrix.m20, matrix.m22, matrix.m23,
        matrix.m30, matrix.m32, matrix.m33);

    float cofactor12 = -DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m03,
        matrix.m20, matrix.m21, matrix.m23,
        matrix.m30, matrix.m31, matrix.m33);

    float cofactor13 = DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m02,
        matrix.m20, matrix.m21, matrix.m22,
        matrix.m30, matrix.m31, matrix.m32);

    float cofactor20 = DETERMINANT3X3(matrix.m01, matrix.m02, matrix.m03,
        matrix.m11, matrix.m12, matrix.m13,
        matrix.m31, matrix.m32, matrix.m33);

    float cofactor21 = -DETERMINANT3X3(matrix.m00, matrix.m02, matrix.m03,
        matrix.m10, matrix.m12, matrix.m13,
        matrix.m30, matrix.m32, matrix.m33);

    float cofactor22 = DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m03,
        matrix.m10, matrix.m11, matrix.m13,
        matrix.m30, matrix.m31, matrix.m33);

    float cofactor23 = -DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m02,
        matrix.m10, matrix.m11, matrix.m12,
        matrix.m30, matrix.m31, matrix.m32);

    float cofactor30 = -DETERMINANT3X3(matrix.m01, matrix.m02, matrix.m03,
        matrix.m11, matrix.m12, matrix.m13,
        matrix.m21, matrix.m22, matrix.m23);

    float cofactor31 = DETERMINANT3X3(matrix.m00, matrix.m02, matrix.m03,
        matrix.m10, matrix.m12, matrix.m13,
        matrix.m20, matrix.m22, matrix.m23);

    float cofactor32 = -DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m03,
        matrix.m10, matrix.m11, matrix.m13,
        matrix.m20, matrix.m21, matrix.m23);

    float cofactor33 = DETERMINANT3X3(matrix.m00, matrix.m01, matrix.m02,
        matrix.m10, matrix.m11, matrix.m12,
        matrix.m20, matrix.m21, matrix.m22);

    return Matrix4f(cofactor00, cofactor10, cofactor20, cofactor30,
                    cofactor01, cofactor11, cofactor21, cofactor31,
                    cofactor02, cofactor12, cofactor22, cofactor32,
                    cofactor03, cofactor13, cofactor23, cofactor33);
}

const Matrix4f Inverse(const Matrix4f& matrix)
{
    float det = determinant(matrix);


    return 1.0f / det * Adjoint(matrix);
}




std::ostream& operator<< (std::ostream& os, const Matrix4f& matrix)
{
    os << matrix.m00 << "  " << matrix.m01 << " " <<  matrix.m02 << " " <<  matrix.m03 <<"\n";
    os << matrix.m10 << "  " << matrix.m11 << " " <<  matrix.m12 << " " <<  matrix.m13 << "\n";
    os << matrix.m20 << "  " << matrix.m21 << " " <<  matrix.m22 << " " <<  matrix.m23 << "\n";
    os << matrix.m30 << "  " << matrix.m31 << " " <<  matrix.m32 << " " <<  matrix.m33 << "\n";
    return os;
}
