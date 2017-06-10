#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <cassert>

static const float EPSILON = 1e-6f;

template <typename T>
bool IsEqual(T left, T right, T epsilon)
{
	return (std::abs(left - right) < epsilon);
}