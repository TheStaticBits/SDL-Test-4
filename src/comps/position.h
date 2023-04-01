#pragma once

#include "utility/vect.h"

namespace Comps
{
	// Stores a position in a Vect<int32_t> object
	struct Position
	{
		Vect<float> pos;
	};
}

namespace Tags
{
	// Objects that are not static and move according to the camera offset
	struct FollowCamera {};
}