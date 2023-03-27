#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "utility/vect.h"
#include "core/window.h"

class Texture
{
public:
	Texture();
	Texture(std::string path, Window& window, const nlohmann::json& constants);
	~Texture();

	inline const Vect<uint32_t> getSize() const { return destSize; }
	inline const Vect<uint32_t> getImgSize() const { return size; }

private:
	SDL_Texture* texture;
	Vect<uint32_t> size;
	Vect<uint32_t> destSize;
};