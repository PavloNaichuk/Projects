#pragma once

#include "Core.h"
#include "Vector.h"
#include "Triangle.h"
#include <vector>

using VertexBuffer = std::vector<Vector>;
using IndexBuffer = std::vector<VertexIndex>;

std::pair<VertexBuffer, IndexBuffer> optimizeTriangleData(const std::vector<Triangle>& triangles);