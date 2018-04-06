#pragma once

#include "Core.h"

class Graph;

std::pair<std::vector<VertexIndex>, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex);

