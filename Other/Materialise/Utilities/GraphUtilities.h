#pragma once

#include "Graph.h"
#include "OptimizeTriangleData.h"
#include <memory>

std::unique_ptr<Graph> createGraph(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer);