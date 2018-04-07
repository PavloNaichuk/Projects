#pragma once

#include "Core.h"

class Graph;

using Path = std::vector<VertexIndex>;
std::pair<Path, bool> findShortestPath(const Graph& graph, VertexIndex startIndex, VertexIndex endIndex);

