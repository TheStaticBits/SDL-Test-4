#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <nlohmann/json.hpp>

#include "vect.h"

class Window 
{
public:
	Window(const nlohmann::json& constants);
	~Window();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vect<uint32_t> winSize;

	uint
};