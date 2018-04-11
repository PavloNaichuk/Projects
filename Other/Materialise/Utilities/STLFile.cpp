#include "stdafx.h"
#include "STLFile.h"
#include <fstream>
#include <cstring>
#include <numeric>

std::pair<std::vector<Triangle>, bool> loadSTLFile(const char* pathToFile)
{
	std::ifstream stream(pathToFile);
	if (!stream)
		return std::make_pair(std::vector<Triangle>(), false);
	
	bool oooppsError = false;
	std::vector<Triangle> triangles;
	
	const std::uint8_t maxCommandLen = 32;
	char command[maxCommandLen];

	Triangle triangle;
	while (true) 
	{
		stream >> command;
		if (!stream)
			break;

		if ((std::strcmp("solid", command) == 0) || (std::strcmp("endsolid", command) == 0))
		{
			stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if (std::strcmp("facet", command) == 0) 
		{
			stream >> command;
			oooppsError = std::strcmp("normal", command) != 0;
			if (oooppsError)
				break;

			stream >> triangle.mNormal.mX;
			oooppsError = stream.bad();
			if (oooppsError)
				break;

			stream >> triangle.mNormal.mY;
			oooppsError = stream.bad();
			if (oooppsError)
				break;

			stream >> triangle.mNormal.mZ;
			oooppsError = stream.bad();
			if (oooppsError)
				break;

			stream >> command;
			oooppsError = std::strcmp("outer", command) != 0;
			if (oooppsError)
				break;

			stream >> command;
			oooppsError = std::strcmp("loop", command) != 0;
			if (oooppsError)
				break;

			Vector* pVertex = &triangle.mVertex1;
			for (int index = 0; index < 3; ++index) 
			{
				stream >> command;
				oooppsError = std::strcmp("vertex", command) != 0;
				if (oooppsError)
					break;

				stream >> pVertex[index].mX;
				oooppsError = stream.bad();
				if (oooppsError)
					break;

				stream >> pVertex[index].mY;
				oooppsError = stream.bad();
				if (oooppsError)
					break;

				stream >> pVertex[index].mZ;
				oooppsError = stream.bad();
				if (oooppsError)
					break;
			}
			if (oooppsError)
				break;

			stream >> command;
			oooppsError = std::strcmp("endloop", command) != 0;
			if (oooppsError)
				break;

			stream >> command;
			oooppsError = std::strcmp("endfacet", command) != 0;
			if (oooppsError)
				break;

			triangles.push_back(triangle);
		}
	}

	if (oooppsError)
		return std::make_pair(std::vector<Triangle>(), false);
	return std::make_pair(triangles, true);
}
