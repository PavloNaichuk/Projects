#pragma once

#include <iostream>
#include "Point3f.h"
#include "Matrix3f.h"

const Matrix3f CreateScalingMatrixAboutOrigin(const float xScale, const float yScale);
const Matrix3f CreateTranslationMatrix(const float xOffset, const float yOffset);
const Matrix3f CreateRotationMatrixAboutOrigin(const float angleInRadians);
const Matrix3f CreateShearingXMatrixAboutOrigin(const float shear);
const Matrix3f CreateShearingYMatrixAboutOrigin(const float shear);


const Matrix3f CreateScalingMatrixAboutPoint(const Point3f& point, float xScale, float yScale);
const Matrix3f CreateRotationMatrixAboutPoint(const Point3f& point, const float angleInRadians);
const Matrix3f CreateShearingXMatrixAboutPoint(const Point3f& point, const float shear);
const Matrix3f CreateShearingYMatrixAboutPoint(const Point3f& point, const float shear);

const Point3f operator* (const Matrix3f& matrix, const Point3f& point);
