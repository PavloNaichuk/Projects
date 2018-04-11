#pragma once

#include "Triangle.h"
#include <vector>

std::pair<std::vector<Triangle>, bool> loadSTLFile(const char* pathToFile);