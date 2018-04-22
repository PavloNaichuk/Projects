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

bool AreEqual(const Triangle& triangle1, const Triangle& triangle2)
{
	return ((areEqual(triangle1.mNormal, triangle2.mNormal)) 
		&& (areEqual(triangle1.mVertex1, triangle2.mVertex1)) 
		&& (areEqual(triangle1.mVertex2, triangle2.mVertex2)) 
		&& (areEqual(triangle1.mVertex3, triangle2.mVertex3)));
}

std::ostream & operator<<(std::ostream& os, const Triangle& triangle)
{
	os << "Normal - " << std::fixed << std::setprecision(1) << triangle.mNormal << "\n";
	os << "Vertex1 - " << std::fixed << std::setprecision(1) << triangle.mVertex1 << " \n";
	os << "Vertex2 - " << std::fixed << std::setprecision(1) << triangle.mVertex2 << " \n";
	os << "Vertex3 - " << std::fixed << std::setprecision(1) << triangle.mVertex3 << " \n";
	return os;
}
