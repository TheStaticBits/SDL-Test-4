#pragma once

#include "utility/vect.h"


namespace Comps
{
	// Objects with movement
	struct Movement
	{
		Vect<float> vel;
	};

	// Objects with acceleration
	struct Acceleration
	{
		Vect<float> acc;
	};

	// Used to store the acceleration speed that the Acceleration component will be set to
	struct StaticAcceleration
	{
		Vect<float> acc;
	};

	// Objects with a max velocity speed
	struct MaxSpeed
	{
		Vect<float> speed;
	};

	// Objects affected by gravity
	struct Gravity
	{
		float strength;
	};
}

namespace Tags
{
	// Movement objects that go off player input
	struct KeyboardInput {};
}