#include "Utilities.h"
#include <cmath>

const float kEpsilon = 1e-6f;

bool equals(float value1, float value2, float epsilon)
{
    return (std::abs(value1 - value2) < epsilon);
}
