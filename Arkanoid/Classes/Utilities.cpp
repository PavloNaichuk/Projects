#include "Utilities.h"

const Size operator* (const Size& lhs, const Size& rhs)
{
	return Size(lhs.width * rhs.width, lhs.height * rhs.height);
}