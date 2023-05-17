#pragma once

#include "utility/vect.h"


namespace Comps
{
	// Objects with movement
	struct Movement
	{
		Vect<float> vel;
		Vect<float> acc;
	};

	// Used to store the acceleration speed that the Acceleration component will be set to when a key is pressed
	struct KeyboardInput
	{
		Vect<float> staticAcc;
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