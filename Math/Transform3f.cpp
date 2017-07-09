#include "Transform3f.h"
#include <cmath>

const Matrix4f CreateScalingMatrixAboutOrigin(const float xScale, const float yScale, const float zScale)
{
    return Matrix4f(xScale, 0.0f, 0.0f, 0.0f,
                    0.0f, yScale, 0.0f, 0.0f,
                    0.0f, 0.0f, zScale, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateTranslationMatrixAboutOrigin(const float xOffset, const float yOffset, const float zOffset)
{
    return Matrix4f(1.0f, 0.0f, 0.0f, xOffset,
                    0.0f, 1.0f, 0.0f, yOffset,
                    0.0f, 0.0f, 1.0f, zOffset,
                    0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateRotationXMatrix(const float angleInRadians)
{
    float sin = std::sinf(angleInRadians);
    float cos = std::cosf(angleInRadians);

    return Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, cos, -sin, 0.0f,
                    0.0f, sin, cos, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

}

const Matrix4f CreateRotationYMatrix(const float angleInRadians)
{
    float sin = std::sinf(angleInRadians);
    float cos = std::cosf(angleInRadians);

    return Matrix4f(cos, 0.0f, sin, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -sin, 0.0f, cos, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

}
const Matrix4f CreateRotationZMatrix(const float angleInRadians)
{
    float sin = std::sinf(angleInRadians);
    float cos = std::cosf(angleInRadians);

    return Matrix4f(cos, -sin, 0.0f, 0.0f,
                    sin, cos, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
}

const Point4f operator* (const Matrix4f& matrix, const Point4f& point)
{
    return Point4f(matrix.m00 * point.mX + matrix.m01 * point.mY + matrix.m02 *  point.mZ + matrix.m03 *  point.mW,
                   matrix.m10 * point.mX + matrix.m11 * point.mY + matrix.m12 *  point.mZ + matrix.m13 *  point.mW,
                   matrix.m20 * point.mX + matrix.m21 * point.mY + matrix.m22 *  point.mZ + matrix.m23 *  point.mW,
                   matrix.m30 * point.mX + matrix.m31 * point.mY + matrix.m32 *  point.mZ + matrix.m33 *  point.mW);
}
