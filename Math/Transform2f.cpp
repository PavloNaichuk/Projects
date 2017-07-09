#include "Transform2f.h"
#include <cmath>

const Matrix3f CreateScalingMatrixAboutOrigin(const float xScale, const float yScale)
{
    return Matrix3f(xScale, 0.0f, 0.0f,
                    0.0f, yScale, 0.0f,
                    0.0f, 0.0f,  1.0f);
}

const Matrix3f CreateTranslationMatrixAboutOrigin(const float xOffset, const float yOffset)
{
    return Matrix3f(1.0f, 0.0f, xOffset,
                    0.0f, 1.0f, yOffset,
                    0.0f, 0.0f, 1.0f);
}

const Matrix3f CreateRotationMatrixAboutOrigin(const float angleInRadians)
{
    float sin = std::sinf(angleInRadians);
    float cos = std::cosf(angleInRadians);

    return Matrix3f(cos, -sin, 0.0f,
                    sin, cos, 0.0f,
                    0.0f, 0.0f, 1.0f);

}

const Matrix3f CreateShearingXMatrixAboutOrigin(const float shear)
{
    return Matrix3f(1.0f, shear, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f);
}

const Matrix3f CreateShearingYMatrixAboutOrigin(const float shear)
{
    return Matrix3f(1.0f,  0.0f, 0.0f,
                    shear, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f);
}

const Matrix3f CreateScalingMatrixAboutPoint(const Point3f& point, float xScale, float yScale)
{
    float pX = point.mX - point.mX * xScale;
    float pY = point.mY - point.mX * yScale;
    return Matrix3f(xScale, 0.0f, pX,
                    0.0f, yScale, pY,
                    0.0f, 0.0f,  1.0f);
}

const Matrix3f CreateRotationMatrixAboutPoint(const Point3f& point, const float angleInRadians)
{
    float sin = std::sinf(angleInRadians);
    float cos = std::cosf(angleInRadians);
    float dX = -cos * point.mX + sin * point.mY + point.mX;
    float dY = -sin * point.mX - cos * point.mY + point.mX;

    return Matrix3f(cos, -sin, dX,
                    sin, cos, dY,
                    0.0f, 0.0f, 1.0f);
}

const Matrix3f CreateShearingXMatrixAboutPoint(const Point3f& point, const float shear)
{
    return Matrix3f(1.0f, shear, -shear * point.mY,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f);
}

const Matrix3f CreateShearingYMatrixAboutPoint(const Point3f& point, const float shear)
{
    return Matrix3f(1.0f,  0.0f, 0.0f,
                    shear, 1.0f, -shear * point.mX,
                    0.0f, 0.0f, 1.0f);
}

const Point3f operator* (const Matrix3f& matrix, const Point3f& point)
{
    return Point3f(matrix.m00 * point.mX + matrix.m01 * point.mY + matrix.m02 *  point.mZ,
                   matrix.m10 * point.mX + matrix.m11 * point.mY + matrix.m12 *  point.mZ,
                   matrix.m20 * point.mX + matrix.m21 * point.mY + matrix.m22 *  point.mZ);
}

