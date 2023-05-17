#pragma once

#include <vector>

#include "comps/texture.h"
#include "utility/vect.h"
#include "comps/offset.h"

namespace Comps
{
	struct TexturePart
	{
		Comps::Offset offset;
		Comps::Texture texture;
	};

	// Used for storing multiple textures in one object
	// each with their own offsets from the base object's position
	struct MultiTexture
	{
		std::vector<Comps::TexturePart> textures;
	};
}