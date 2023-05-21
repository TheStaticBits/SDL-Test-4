#pragma once

namespace Comps
{
	// Player-specific components
	struct Player {
		float bounceVelocity;
		Vect<float> boundaries;
	};
}