#include "stdafx.h"
#include "Triangle.h"
#include <iomanip>

Triangle::Triangle(const Vector& normal, const Vector& vertex1, const Vector& vertex2, const Vector& vertex3)
	: mNormal(normal)
	, mVertex1(vertex1)
	, mVertex2(vertex2)
	, mVertex3(vertex3)
{
}

std::ostream & operator<<(std::ostream& os, const Triangle& triangle)
{
	os << "Normal - " << std::fixed << std::setprecision(1) << triangle.mNormal << "\n";
	os << "Vertex1 - " << std::fixed << std::setprecision(1) << triangle.mVertex1 << " \n";
	os << "Vertex2 - " << std::fixed << std::setprecision(1) << triangle.mVertex2 << " \n";
	os << "Vertex3 - " << std::fixed << std::setprecision(1) << triangle.mVertex3 << " \n";
	return os;
}
