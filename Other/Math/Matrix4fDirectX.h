#pragma once

#include "Matrix4f.h"

struct Point3f;
struct Vector3f;

Matrix4f CreateLookAtMatrix(const Point3f& worldCameraPos, const Point3f& worldCameraFocus, const Vector3f& worldUp);
