#pragma once

#include "Point4f.h"
#include "Matrix4f.h"

const Matrix4f CreateScalingMatrixAboutOrigin(const float xScale, const float yScale, const float zScale);
const Matrix4f CreateTranslationMatrixAboutOrigin(const float xOffset, const float yOffset, const float zOffset);
const Matrix4f CreateRotationXMatrix(const float angleInRadians);
const Matrix4f CreateRotationYMatrix(const float angleInRadians);
const Matrix4f CreateRotationZMatrix(const float angleInRadians);

const Point4f operator* (const Matrix4f& matrix, const Point4f& point);
