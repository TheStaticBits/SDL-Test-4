#include "comp/texture.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "utility/vect.h"
#include "core/window.h"

Texture::Texture()
	: texture(nullptr)
{

}

Texture::Texture(std::string path, Window& window, const nlohmann::json& constants)
	: texture(nullptr)
{
	// Loading texture
	texture = window.loadTexture(path);

	// Getting scale from constants
	uint32_t scale = constants["display"]["scale"].get<uint32_t>();

	// Get size of texture
	int x, y;
	SDL_QueryTexture(texture, NULL, NULL, &x, &y);
	size = Vect<uint32_t>(x, y);
	destSize = size * scale;
}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}