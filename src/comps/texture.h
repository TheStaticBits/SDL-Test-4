#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "utility/vect.h"
#include "core/window.h"

namespace Comps
{
	struct Texture
	{
		SDL_Texture* tex;
		Vect<uint32_t> size;
		Vect<uint32_t> destSize;
	};
}
