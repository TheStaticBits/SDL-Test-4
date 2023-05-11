#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "comps/texture.h"

namespace Comps
{
	// Stores textures in a map (used for tile textures)
	struct TextureStorage
	{
		std::unordered_map<std::string, Comps::Texture> textures;
	};
}