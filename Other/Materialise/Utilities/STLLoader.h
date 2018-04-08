#pragma once

#include "Triangle.h"
#include <vector>

class STLLoader
{
public:
	std::pair<std::vector<Triangle>, bool> load(const char* pathToFile);
};
