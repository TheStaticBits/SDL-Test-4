#pragma once

#include "utility/vect.h"

namespace Comps
{
	// Stores an offset in a Vect<int32_t> object
	struct Offset
	{
		Vect<int32_t> offset;
	};
}

namespace Tags
{
	// Camera object's tag
	struct Camera {};
}