#pragma once

/* Used for storing multiple textures in one object */

#include <vector>

#include "comps/texture.h"
#include "utility/vect.h"

namespace Comps
{
	struct MultiTexture
	{
		// Texture and offset from object base position
		std::vector<std::pair<Comps::Texture, Vect<int32_t>>> textures;
	};
}