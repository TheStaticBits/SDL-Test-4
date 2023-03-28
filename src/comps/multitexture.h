#pragma once

/* Used for storing multiple textures in one object */

#include <vector>
#include "comps/texture.h"

namespace Comps
{
	struct MultiTexture
	{
		std::vector<Comps::Texture> textures;
	};
};