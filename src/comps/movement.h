#pragma once

/* Objects with movement */

#include "utility/vect.h"

namespace Comps
{
	struct Movement
	{
		Vect<float> vel;
		float acceleration;
		float maxSpeed;
	};
}

// Movement objects that go off player input
namespace Tags
{
	struct KeyboardInput {};
}