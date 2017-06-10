#pragma once

#include "Matrix4f.h"

struct Vector3f;

const Matrix4f CreateTranslationMatrix(const Vector3f& offset);
const Matrix4f CreateTranslationMatrix(float xOffset, float yOffset, float zOffset);

const Matrix4f CreateScalingMatrix(const Vector3f& scale);
const Matrix4f CreateScalingMatrix(float xScale, float yScale, float zScale);
const Matrix4f CreateScalingMatrix(float scale);

const Matrix4f CreateRotationXMatrix(float angleInRadians);
const Matrix4f CreateRotationYMatrix(float angleInRadians);
const Matrix4f CreateRotationZMatrix(float angleInRadians);

const Matrix4f CreateLookAtMatrix(const Vector3f& eyePos, const Vector3f& lookAtPos, const Vector3f& upDir);

const Matrix4f CreateOrthoOffCenterProjMatrix(float leftX, float rightX, float bottomY, float topY, float nearZ, float farZ);
const Matrix4f CreateOrthoProjMatrix(float width, float height, float nearZ, float farZ);

const Matrix4f CreatePerspectiveProjMatrix(float nearWidth, float nearHeight, float nearZ, float farZ);
const Matrix4f CreatePerspectiveFovProjMatrix(float fovYInRadians, float aspectRatio, float nearZ, float farZ);
