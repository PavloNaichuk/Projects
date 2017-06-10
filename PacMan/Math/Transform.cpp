#include "Transform.h"
#include "Vector3f.h"

const Matrix4f CreateTranslationMatrix(const Vector3f& offset)
{
	return CreateTranslationMatrix(offset.m_X, offset.m_Y, offset.m_Z);
}

const Matrix4f CreateTranslationMatrix(float xOffset, float yOffset, float zOffset)
{
	return Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					xOffset, yOffset, zOffset, 1.0f);
}

const Matrix4f CreateScalingMatrix(const Vector3f& scale)
{
	return CreateScalingMatrix(scale.m_X, scale.m_Y, scale.m_Z);
}

const Matrix4f CreateScalingMatrix(float xScale, float yScale, float zScale)
{
	return Matrix4f(xScale, 0.0f, 0.0f, 0.0f,
					0.0f, yScale, 0.0f, 0.0f,
					0.0f, 0.0f, zScale, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateScalingMatrix(float scale)
{
	return CreateScalingMatrix(scale, scale, scale);
}

const Matrix4f CreateRotationXMatrix(float angleInRadians)
{
	float sinAngle = std::sin(angleInRadians);
	float cosAngle = std::cos(angleInRadians);

	return Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, cosAngle, sinAngle, 0.0f,
					0.0f, -sinAngle, cosAngle, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateRotationYMatrix(float angleInRadians)
{
	float sinAngle = std::sin(angleInRadians);
	float cosAngle = std::cos(angleInRadians);

	return Matrix4f(cosAngle, 0.0f, -sinAngle, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					sinAngle, 0.0f, cosAngle, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateRotationZMatrix(float angleInRadians)
{
	float sinAngle = std::sin(angleInRadians);
	float cosAngle = std::cos(angleInRadians);

	return Matrix4f(cosAngle, sinAngle, 0.0f, 0.0f,
				   -sinAngle, cosAngle, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4f CreateLookAtMatrix(const Vector3f& eyePos, const Vector3f& lookAtPos, const Vector3f& upDir)
{
	Vector3f zAxis = Normalize(lookAtPos - eyePos);
	Vector3f xAxis = Normalize(Cross(upDir, zAxis));
	Vector3f yAxis = Cross(zAxis, xAxis);

	return Matrix4f(xAxis.m_X, yAxis.m_X, zAxis.m_X, 0.0f,
					xAxis.m_Y, yAxis.m_Y, zAxis.m_Y, 0.0f,
					xAxis.m_Z, yAxis.m_Z, zAxis.m_Z, 0.0f,
				   -Dot(xAxis, eyePos), -Dot(yAxis, eyePos), -Dot(zAxis, eyePos), 1.0f);
}

const Matrix4f CreateOrthoOffCenterProjMatrix(float leftX, float rightX, float bottomY, float topY, float nearZ, float farZ)
{
	return Matrix4f(2.0f / (rightX - leftX), 0.0f, 0.0f, 0.0f,
					0.0f, 2.0f / (topY - bottomY), 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f,
				   (leftX + rightX) / (leftX - rightX), (topY + bottomY) / (bottomY - topY), nearZ / (nearZ / farZ), 1.0f);
}

const Matrix4f CreateOrthoProjMatrix(float width, float height, float nearZ, float farZ)
{
	return Matrix4f(2.0f / width, 0.0f, 0.0f, 0.0f,
					0.0f, 2.0f / height, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f,
					0.0f, 0.0f, -nearZ / (farZ - nearZ), 1.0f);
}

const Matrix4f CreatePerspectiveProjMatrix(float nearWidth, float nearHeight, float nearZ, float farZ)
{
	return Matrix4f(2.0f * nearZ / nearWidth, 0.0f, 0.0f, 0.0f,
					0.0f, 2 * nearZ / nearHeight, 0.0f, 0.0f,
					0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f,
					0.0f, 0.0f, nearZ * farZ / (nearZ - farZ), 0.0f);
}

const Matrix4f CreatePerspectiveFovProjMatrix(float fovYInRadians, float aspectRatio, float nearZ, float farZ)
{
	float yScale = 1.0f / (std::tanf(0.5f * fovYInRadians));
	float xScale = yScale / aspectRatio;

	return Matrix4f(xScale, 0.0f, 0.0f, 0.0f,
					0.0f, yScale, 0.0f, 0.0f,
					0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f,
					0.0f, 0.0f, nearZ * farZ / (nearZ - farZ), 0.0f);
}
