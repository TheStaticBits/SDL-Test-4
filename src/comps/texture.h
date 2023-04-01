#pragma once

#include <SDL2/SDL.h>
#include "utility/vect.h"

namespace Comps
{
	// Stores an SDL_Texture and its source size and destination size rectangles for rendering
	struct Texture
	{
		SDL_Texture* tex;
		Vect<uint32_t> src;
		Vect<uint32_t> destSize;
	};
}