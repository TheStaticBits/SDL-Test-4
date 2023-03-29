#pragma once

#include <SDL2/SDL.h>
#include "utility/vect.h"

namespace Comps
{
	struct Texture
	{
		SDL_Texture* tex;
		Vect<uint32_t> size;
		Vect<uint32_t> destSize;
	};
}
