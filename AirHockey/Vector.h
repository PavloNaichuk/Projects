#pragma once

struct Vector
{
	Vector();
	Vector(float x, float y);
	
	const Vector operator- () const;

	float mX, mY;
};

using Point = Vector;
using Size = Vector;

const Vector operator+ (const Vector& vec1, const Vector& vec2);
const Vector operator+ (float scalar, const Vector& vec);
const Vector operator+ (const Vector& vec, float scalar);

const Vector operator- (const Vector& vec1, const Vector& vec2);
const Vector operator- (float scalar, const Vector& vec);
const Vector operator- (const Vector& vec, float scalar);

const Vector operator* (const Vector& vec, float scalar);
const Vector operator* (float scalar, const Vector& vec);
const Vector operator* (const Vector& vec1, const Vector& vec2);
const Vector operator/ (const Vector& vec, float scalar);
const Vector operator/ (float scalar, const Vector& vec);

const Vector& operator+= (Vector& vec1, const Vector& vec2);
const Vector& operator-= (Vector& vec1, const Vector& vec2);
const Vector& operator*= (Vector& vec1, const Vector& vec2);
const Vector& operator*= (Vector& vec, float scalar);

const Vector Perp(const Vector& vec);
const Vector Reflect(const Vector& incident, const Vector& normal);
const Vector Normalize(const Vector& vec);
bool AreEqual(float value1, float value2);
float Dot(const Vector& vec1, const Vector& vec2);
float Length(const Vector& vec);
float SquaredLength(const Vector& vec);
float Distance(const Point& point1, const Point& point2);
float SquaredDistance(const Point& point1, const Point& point2);

struct Ray
{
	Ray(const Point& origin, const Vector& direction);

	Point mOrigin;
	Vector mDirection;
};

const Point CalcPointOnRay(const Ray& ray, float param);
bool TestHit(const Ray& ray1, const Ray& ray2, float& hitParam1, float& hitParam2);

float Square(float value);
bool CirclesHitEachOther(const Point& center1, float radius1, const Point& center2, float radius2);
