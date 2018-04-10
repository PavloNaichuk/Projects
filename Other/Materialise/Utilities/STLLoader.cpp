#include "stdafx.h"
#include "STLLoader.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <algorithm>



std::pair<std::vector<Triangle>, bool> STLLoader::load(const char* pathToFile)
{
	std::vector<Triangle> vector;
	Triangle triangle;
	//Vector normal;
	Vector vertex;
	std::vector<Vector> vectorVertex;
	
	std::ifstream stream(pathToFile);
	const std::uint8_t maxCommandLen = 32;
	char command[maxCommandLen];
	while (true)
	{
		stream >> command;
		if (!stream)
		{
			break;
			return std::make_pair(std::vector<Triangle>(), false);
		}
		if (std::strcmp("normal", command) == 0) 
		{
			float x, y, z;
			stream >> x >> y >> z;

			triangle.mNormal.mX = x;
			triangle.mNormal.mY = y;
			triangle.mNormal.mZ = z;

		}
		else if (std::strcmp("vertex", command) == 0) 
		{
			float x, y, z;
			stream >> x >> y >> z;

			vertex.mX = x;
			vertex.mY = y;
			vertex.mZ = z;
			vectorVertex.push_back(vertex);
		}

		else if (std::strcmp("endloop", command) == 0)
		{
			//triangle.mNormal = normal;
			triangle.mVertex1 = vectorVertex[0];
			triangle.mVertex2 = vectorVertex[1];
			triangle.mVertex3 = vectorVertex[2];

			vector.push_back(triangle);
			vectorVertex.clear();
		}
	}
	
	/*
	Vector normal;
	std::string line;
	std::string result;
	Triangle triangle;

	std::vector<Vector> vertex;
	while (std::getline(stream, line))
	{
		std::vector < std::string> str;
		std::istringstream isString(line);
		while (std::getline(isString, result, ' ')) 
		{
			if (result != "")
				str.push_back(result);
		}
		if (str[0] == "facet")
		{
			normal.mX = std::stof(str[2]);
			normal.mY = std::stof(str[3]);
			normal.mZ = std::stof(str[4]);
		}
		else if (str[0] == "vertex")
		{
			Vector tempVertex;
			tempVertex.mX = std::stof(str[1]);
			tempVertex.mY = std::stof(str[2]);
			tempVertex.mZ = std::stof(str[3]);
			vertex.push_back(tempVertex);
		}
		else if (str[0] == "endloop")
		{
			triangle.mNormal = normal;
			triangle.mVertex1 = vertex[0];
			triangle.mVertex2 = vertex[1];
			triangle.mVertex3 = vertex[2];

			vector.push_back(triangle);
			vertex.clear();
		}
	}*/
	return std::make_pair(vector, true);
}
