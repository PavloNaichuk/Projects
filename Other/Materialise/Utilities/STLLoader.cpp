#include "stdafx.h"
#include "STLLoader.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <algorithm>


std::pair<std::vector<Triangle>, bool> STLLoader::load(const char* pathToFile)
{
	std::vector<Triangle> vector;
	
	Vector normal;
	std::vector<Vector> vertex;

	Triangle triangle;
	std::ifstream stream(pathToFile);
	if (!stream)
	{
		return std::make_pair(std::vector<Triangle>(), false);
	}
	std::string line;
	std::string result;
	while (std::getline(stream, line))
	{
		std::vector < std::string> temp;
		std::istringstream isString(line);
		while (std::getline(isString, result, ' ')) 
		{
			if (result != "")
				temp.push_back(result);
		}
		if (temp[0] == "facet")
		{
			normal.mX = std::stof(temp[2]);
			normal.mY = std::stof(temp[3]);
			normal.mZ = std::stof(temp[4]);
		}
		else if (temp[0] == "vertex") 
		{
			Vector tempVertex;
			tempVertex.mX = std::stof(temp[1]);
			tempVertex.mY = std::stof(temp[2]);
			tempVertex.mZ = std::stof(temp[3]);
			vertex.push_back(tempVertex);
		}
		else if (temp[0] == "endloop")
		{
			triangle.mNormal = normal;
			triangle.mVertex1 = vertex[0];
			triangle.mVertex2 = vertex[1];
			triangle.mVertex3 = vertex[2];

			vector.push_back(triangle);
			vertex.clear();
		}
	}
	return std::make_pair(vector, true);
}
