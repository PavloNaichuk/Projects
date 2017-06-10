#include "Matrix4f.h"

#define DETERMINANT2X2(m00, m01, m10, m11) \
    (m00 * m11 - m01 * m10)

#define DETERMINANT3X3(m00, m01, m02, m10, m11, m12, m20, m21, m22) \
	(m20 * (m01 * m12 - m02 * m11) + \
     m21 * (m02 * m10 - m00 * m12) + \
     m22 * (m00 * m11 - m01 * m10))

const Matrix4f Matrix4f::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
								  0.0f, 1.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 1.0f, 0.0f,
								  0.0f, 0.0f, 0.0f, 1.0f);

const Matrix4f Matrix4f::ZERO(0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f);

Matrix4f::Matrix4f()
    : m_00(0.0f), m_01(0.0f), m_02(0.0f), m_03(0.0f)
    , m_10(0.0f), m_11(0.0f), m_12(0.0f), m_13(0.0f)
    , m_20(0.0f), m_21(0.0f), m_22(0.0f), m_23(0.0f)
    , m_30(0.0f), m_31(0.0f), m_32(0.0f), m_33(0.0f)
{
}

Matrix4f::Matrix4f(float scalar)
    : m_00(scalar), m_01(scalar), m_02(scalar), m_03(scalar)
    , m_10(scalar), m_11(scalar), m_12(scalar), m_13(scalar)
    , m_20(scalar), m_21(scalar), m_22(scalar), m_23(scalar)
    , m_30(scalar), m_31(scalar), m_32(scalar), m_33(scalar)
{
}

Matrix4f::Matrix4f(float scalar00, float scalar01, float scalar02, float scalar03,
                   float scalar10, float scalar11, float scalar12, float scalar13,
                   float scalar20, float scalar21, float scalar22, float scalar23,
                   float scalar30, float scalar31, float scalar32, float scalar33)
    : m_00(scalar00), m_01(scalar01), m_02(scalar02), m_03(scalar03)
    , m_10(scalar10), m_11(scalar11), m_12(scalar12), m_13(scalar13)
    , m_20(scalar20), m_21(scalar21), m_22(scalar22), m_23(scalar23)
    , m_30(scalar30), m_31(scalar31), m_32(scalar32), m_33(scalar33)
{
}

const Matrix4f Matrix4f::operator- () const
{
	return Matrix4f(-m_00, -m_01, -m_02, -m_03,
					-m_10, -m_11, -m_12, -m_13,
					-m_20, -m_21, -m_22, -m_23,
					-m_30, -m_31, -m_32, -m_33);
}

Matrix4f& operator+= (Matrix4f& matrix1, const Matrix4f& matrix2)
{
	matrix1.m_00 += matrix2.m_00;
	matrix1.m_01 += matrix2.m_01;
	matrix1.m_02 += matrix2.m_02;
	matrix1.m_03 += matrix2.m_03;

	matrix1.m_10 += matrix2.m_10;
	matrix1.m_11 += matrix2.m_11;
	matrix1.m_12 += matrix2.m_12;
	matrix1.m_13 += matrix2.m_13;

	matrix1.m_20 += matrix2.m_20;
	matrix1.m_21 += matrix2.m_21;
	matrix1.m_22 += matrix2.m_22;
	matrix1.m_23 += matrix2.m_23;

	matrix1.m_30 += matrix2.m_30;
	matrix1.m_31 += matrix2.m_31;
	matrix1.m_32 += matrix2.m_32;
	matrix1.m_33 += matrix2.m_33;

    return matrix1;
}

Matrix4f& operator-= (Matrix4f& matrix1, const Matrix4f& matrix2)
{
	matrix1.m_00 -= matrix2.m_00;
	matrix1.m_01 -= matrix2.m_01;
	matrix1.m_02 -= matrix2.m_02;
	matrix1.m_03 -= matrix2.m_03;

	matrix1.m_10 -= matrix2.m_10;
	matrix1.m_11 -= matrix2.m_11;
	matrix1.m_12 -= matrix2.m_12;
	matrix1.m_13 -= matrix2.m_13;

	matrix1.m_20 -= matrix2.m_20;
	matrix1.m_21 -= matrix2.m_21;
	matrix1.m_22 -= matrix2.m_22;
	matrix1.m_23 -= matrix2.m_23;

	matrix1.m_30 -= matrix2.m_30;
	matrix1.m_31 -= matrix2.m_31;
	matrix1.m_32 -= matrix2.m_32;
	matrix1.m_33 -= matrix2.m_33;

    return matrix1;
}

Matrix4f& operator*= (Matrix4f& matrix1, const Matrix4f& matrix2)
{
    float m00 = matrix1.m_00 * matrix2.m_00 + matrix1.m_01 * matrix2.m_10 + matrix1.m_02 * matrix2.m_20 + matrix1.m_03 * matrix2.m_30;
    float m01 = matrix1.m_00 * matrix2.m_01 + matrix1.m_01 * matrix2.m_11 + matrix1.m_02 * matrix2.m_21 + matrix1.m_03 * matrix2.m_31;
    float m02 = matrix1.m_00 * matrix2.m_02 + matrix1.m_01 * matrix2.m_12 + matrix1.m_02 * matrix2.m_22 + matrix1.m_03 * matrix2.m_32;
    float m03 = matrix1.m_00 * matrix2.m_03 + matrix1.m_01 * matrix2.m_13 + matrix1.m_02 * matrix2.m_23 + matrix1.m_03 * matrix2.m_33;
        
    float m10 = matrix1.m_10 * matrix2.m_00 + matrix1.m_11 * matrix2.m_10 + matrix1.m_12 * matrix2.m_20 + matrix1.m_13 * matrix2.m_30;
    float m11 = matrix1.m_10 * matrix2.m_01 + matrix1.m_11 * matrix2.m_11 + matrix1.m_12 * matrix2.m_21 + matrix1.m_13 * matrix2.m_31;
    float m12 = matrix1.m_10 * matrix2.m_02 + matrix1.m_11 * matrix2.m_12 + matrix1.m_12 * matrix2.m_22 + matrix1.m_13 * matrix2.m_32;
    float m13 = matrix1.m_10 * matrix2.m_03 + matrix1.m_11 * matrix2.m_13 + matrix1.m_12 * matrix2.m_23 + matrix1.m_13 * matrix2.m_33;

    float m20 = matrix1.m_20 * matrix2.m_00 + matrix1.m_21 * matrix2.m_10 + matrix1.m_22 * matrix2.m_20 + matrix1.m_23 * matrix2.m_30;
    float m21 = matrix1.m_20 * matrix2.m_01 + matrix1.m_21 * matrix2.m_11 + matrix1.m_22 * matrix2.m_21 + matrix1.m_23 * matrix2.m_31;
    float m22 = matrix1.m_20 * matrix2.m_02 + matrix1.m_21 * matrix2.m_12 + matrix1.m_22 * matrix2.m_22 + matrix1.m_23 * matrix2.m_32;
    float m23 = matrix1.m_20 * matrix2.m_03 + matrix1.m_21 * matrix2.m_13 + matrix1.m_22 * matrix2.m_23 + matrix1.m_23 * matrix2.m_33;

    float m30 = matrix1.m_30 * matrix2.m_00 + matrix1.m_31 * matrix2.m_10 + matrix1.m_32 * matrix2.m_20 + matrix1.m_33 * matrix2.m_30;
    float m31 = matrix1.m_30 * matrix2.m_01 + matrix1.m_31 * matrix2.m_11 + matrix1.m_32 * matrix2.m_21 + matrix1.m_33 * matrix2.m_31;
    float m32 = matrix1.m_30 * matrix2.m_02 + matrix1.m_31 * matrix2.m_12 + matrix1.m_32 * matrix2.m_22 + matrix1.m_33 * matrix2.m_32;
    float m33 = matrix1.m_30 * matrix2.m_03 + matrix1.m_31 * matrix2.m_13 + matrix1.m_32 * matrix2.m_23 + matrix1.m_33 * matrix2.m_33;

	matrix1.m_00 = m00;
	matrix1.m_01 = m01;
	matrix1.m_02 = m02;
	matrix1.m_03 = m03;

	matrix1.m_10 = m10;
	matrix1.m_11 = m11;
	matrix1.m_12 = m12;
	matrix1.m_13 = m13;

	matrix1.m_20 = m20;
	matrix1.m_21 = m21;
	matrix1.m_22 = m22;
	matrix1.m_23 = m23;

	matrix1.m_30 = m30;
	matrix1.m_31 = m31;
	matrix1.m_32 = m32;
	matrix1.m_33 = m33;

    return matrix1;
}

Matrix4f& operator+= (Matrix4f& matrix, float scalar)
{
	matrix.m_00 += scalar;
	matrix.m_01 += scalar;
	matrix.m_02 += scalar;
	matrix.m_03 += scalar;

	matrix.m_10 += scalar;
	matrix.m_11 += scalar;
	matrix.m_12 += scalar;
	matrix.m_13 += scalar;

	matrix.m_20 += scalar;
	matrix.m_21 += scalar;
	matrix.m_22 += scalar;
	matrix.m_23 += scalar;

	matrix.m_30 += scalar;
	matrix.m_31 += scalar;
	matrix.m_32 += scalar;
	matrix.m_33 += scalar;

    return matrix;
}

Matrix4f& operator-= (Matrix4f& matrix, float scalar)
{
	matrix.m_00 -= scalar;
	matrix.m_01 -= scalar;
	matrix.m_02 -= scalar;
	matrix.m_03 -= scalar;

	matrix.m_10 -= scalar;
	matrix.m_11 -= scalar;
	matrix.m_12 -= scalar;
	matrix.m_13 -= scalar;

	matrix.m_20 -= scalar;
	matrix.m_21 -= scalar;
	matrix.m_22 -= scalar;
	matrix.m_23 -= scalar;

	matrix.m_30 -= scalar;
	matrix.m_31 -= scalar;
	matrix.m_32 -= scalar;
	matrix.m_33 -= scalar;

    return matrix;
}

Matrix4f& operator*= (Matrix4f& matrix, float scalar)
{
	matrix.m_00 *= scalar;
	matrix.m_01 *= scalar;
	matrix.m_02 *= scalar;
	matrix.m_03 *= scalar;

	matrix.m_10 *= scalar;
	matrix.m_11 *= scalar;
	matrix.m_12 *= scalar;
	matrix.m_13 *= scalar;

	matrix.m_20 *= scalar;
	matrix.m_21 *= scalar;
	matrix.m_22 *= scalar;
	matrix.m_23 *= scalar;

	matrix.m_30 *= scalar;
	matrix.m_31 *= scalar;
	matrix.m_32 *= scalar;
	matrix.m_33 *= scalar;

    return matrix;
}

const Matrix4f operator+ (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return (Matrix4f(matrix1) += matrix2);
}

const Matrix4f operator- (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return (Matrix4f(matrix1) -= matrix2);
}

const Matrix4f operator* (const Matrix4f& matrix1, const Matrix4f& matrix2)
{
    return (Matrix4f(matrix1) *= matrix2);
}

const Matrix4f operator+ (const Matrix4f& matrix, float scalar)
{
    return (Matrix4f(matrix) += scalar);
}

const Matrix4f operator+ (float scalar, const Matrix4f& matrix)
{
	return (Matrix4f(matrix) += scalar);
}

const Matrix4f operator- (const Matrix4f& matrix, float scalar)
{
    return (Matrix4f(matrix) -= scalar);
}

const Matrix4f operator- (float scalar, const Matrix4f& matrix)
{
	return Matrix4f(scalar - matrix.m_00, scalar - matrix.m_01, scalar - matrix.m_02, scalar - matrix.m_03,
					scalar - matrix.m_10, scalar - matrix.m_11, scalar - matrix.m_12, scalar - matrix.m_13,
					scalar - matrix.m_20, scalar - matrix.m_21, scalar - matrix.m_22, scalar - matrix.m_23,
					scalar - matrix.m_30, scalar - matrix.m_31, scalar - matrix.m_32, scalar - matrix.m_33);
}

const Matrix4f operator* (const Matrix4f& matrix, float scalar)
{
    return (Matrix4f(matrix) *= scalar);
}

const Matrix4f operator* (float scalar, const Matrix4f& matrix)
{
	return (Matrix4f(matrix) *= scalar);
}

const Vector4f operator* (const Vector4f& vec, const Matrix4f& matrix)
{
	float x = vec.m_X * matrix.m_00 + vec.m_Y * matrix.m_10 + vec.m_Z * matrix.m_20 + vec.m_W * matrix.m_30;
	float y = vec.m_X * matrix.m_01 + vec.m_Y * matrix.m_11 + vec.m_Z * matrix.m_21 + vec.m_W * matrix.m_31;
	float z = vec.m_X * matrix.m_02 + vec.m_Y * matrix.m_12 + vec.m_Z * matrix.m_22 + vec.m_W * matrix.m_32;
	float w = vec.m_X * matrix.m_03 + vec.m_Y * matrix.m_13 + vec.m_Z * matrix.m_23 + vec.m_W * matrix.m_33;

	return Vector4f(x, y, z, w);
}

const Vector4f operator* (const Matrix4f& matrix, const Vector4f& vec)
{
	float x = vec.m_X * matrix.m_00 + vec.m_Y * matrix.m_01 + vec.m_Z * matrix.m_02 + vec.m_W * matrix.m_03;
	float y = vec.m_X * matrix.m_10 + vec.m_Y * matrix.m_11 + vec.m_Z * matrix.m_12 + vec.m_W * matrix.m_13;
	float z = vec.m_X * matrix.m_20 + vec.m_Y * matrix.m_21 + vec.m_Z * matrix.m_22 + vec.m_W * matrix.m_23;
	float w = vec.m_X * matrix.m_30 + vec.m_Y * matrix.m_31 + vec.m_Z * matrix.m_32 + vec.m_W * matrix.m_33;

	return Vector4f(x, y, z, w);
}

const Matrix4f Transpose(const Matrix4f& matrix)
{
    return Matrix4f(matrix.m_00, matrix.m_10, matrix.m_20, matrix.m_30,
                    matrix.m_01, matrix.m_11, matrix.m_21, matrix.m_31,
                    matrix.m_02, matrix.m_12, matrix.m_22, matrix.m_32,
                    matrix.m_03, matrix.m_13, matrix.m_23, matrix.m_33);
}

float Determinant(const Matrix4f& matrix)
{
	float cofactor00 = DETERMINANT3X3(matrix.m_11, matrix.m_12, matrix.m_13,
		matrix.m_21, matrix.m_22, matrix.m_23,
		matrix.m_31, matrix.m_32, matrix.m_33);

	float cofactor01 = -DETERMINANT3X3(matrix.m_10, matrix.m_12, matrix.m_13,
		matrix.m_20, matrix.m_22, matrix.m_23,
		matrix.m_30, matrix.m_32, matrix.m_33);

	float cofactor02 = DETERMINANT3X3(matrix.m_10, matrix.m_11, matrix.m_13,
		matrix.m_20, matrix.m_21, matrix.m_23,
		matrix.m_30, matrix.m_31, matrix.m_33);

	float cofactor03 = -DETERMINANT3X3(matrix.m_10, matrix.m_11, matrix.m_12,
		matrix.m_20, matrix.m_21, matrix.m_22,
		matrix.m_30, matrix.m_31, matrix.m_32);

    return (matrix.m_00 * cofactor00
		  + matrix.m_01 * cofactor01
		  + matrix.m_02 * cofactor02
		  + matrix.m_03 * cofactor03);
}

const Matrix4f Adjoint(const Matrix4f& matrix)
{
	float cofactor00 = DETERMINANT3X3(matrix.m_11, matrix.m_12, matrix.m_13,
		matrix.m_21, matrix.m_22, matrix.m_23,
		matrix.m_31, matrix.m_32, matrix.m_33);

	float cofactor01 = -DETERMINANT3X3(matrix.m_10, matrix.m_12, matrix.m_13,
		matrix.m_20, matrix.m_22, matrix.m_23,
		matrix.m_30, matrix.m_32, matrix.m_33);

	float cofactor02 = DETERMINANT3X3(matrix.m_10, matrix.m_11, matrix.m_13,
		matrix.m_20, matrix.m_21, matrix.m_23,
		matrix.m_30, matrix.m_31, matrix.m_33);

	float cofactor03 = -DETERMINANT3X3(matrix.m_10, matrix.m_11, matrix.m_12,
		matrix.m_20, matrix.m_21, matrix.m_22,
		matrix.m_30, matrix.m_31, matrix.m_32);

	float cofactor10 = -DETERMINANT3X3(matrix.m_01, matrix.m_02, matrix.m_03,
		matrix.m_21, matrix.m_22, matrix.m_23,
		matrix.m_31, matrix.m_32, matrix.m_33);

	float cofactor11 = DETERMINANT3X3(matrix.m_00, matrix.m_02, matrix.m_03,
		matrix.m_20, matrix.m_22, matrix.m_23,
		matrix.m_30, matrix.m_32, matrix.m_33);

	float cofactor12 = -DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_03,
		matrix.m_20, matrix.m_21, matrix.m_23,
		matrix.m_30, matrix.m_31, matrix.m_33);

	float cofactor13 = DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_02,
		matrix.m_20, matrix.m_21, matrix.m_22,
		matrix.m_30, matrix.m_31, matrix.m_32);

	float cofactor20 = DETERMINANT3X3(matrix.m_01, matrix.m_02, matrix.m_03,
		matrix.m_11, matrix.m_12, matrix.m_13,
		matrix.m_31, matrix.m_32, matrix.m_33);

	float cofactor21 = -DETERMINANT3X3(matrix.m_00, matrix.m_02, matrix.m_03,
		matrix.m_10, matrix.m_12, matrix.m_13,
		matrix.m_30, matrix.m_32, matrix.m_33);

	float cofactor22 = DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_03,
		matrix.m_10, matrix.m_11, matrix.m_13,
		matrix.m_30, matrix.m_31, matrix.m_33);

	float cofactor23 = -DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_02,
		matrix.m_10, matrix.m_11, matrix.m_12,
		matrix.m_30, matrix.m_31, matrix.m_32);

	float cofactor30 = -DETERMINANT3X3(matrix.m_01, matrix.m_02, matrix.m_03,
		matrix.m_11, matrix.m_12, matrix.m_13,
		matrix.m_21, matrix.m_22, matrix.m_23);

	float cofactor31 = DETERMINANT3X3(matrix.m_00, matrix.m_02, matrix.m_03,
		matrix.m_10, matrix.m_12, matrix.m_13,
		matrix.m_20, matrix.m_22, matrix.m_23);

	float cofactor32 = -DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_03,
		matrix.m_10, matrix.m_11, matrix.m_13,
		matrix.m_20, matrix.m_21, matrix.m_23);

	float cofactor33 = DETERMINANT3X3(matrix.m_00, matrix.m_01, matrix.m_02,
		matrix.m_10, matrix.m_11, matrix.m_12,
		matrix.m_20, matrix.m_21, matrix.m_22);

	return Matrix4f(cofactor00, cofactor10, cofactor20, cofactor30,
					cofactor01, cofactor11, cofactor21, cofactor31,
					cofactor02, cofactor12, cofactor22, cofactor32,
					cofactor03, cofactor13, cofactor23, cofactor33);
}

const Matrix4f Inverse(const Matrix4f& matrix)
{
	float det = Determinant(matrix);
	assert(!IsEqual(det, 0.0f, EPSILON));
	
	return (1.0f / det) * Adjoint(matrix);
}
