#pragma once

#include <vector>

#include "comps/texture.h"
#include "utility/vect.h"
#include "comps/offset.h"

namespace Comps
{
	// Used for storing multiple textures in one object
	struct MultiTexture
	{
		// Texture and offset from object base position
		std::vector<std::pair<Comps::Texture, Comps::Offset>> textures;
	};
}