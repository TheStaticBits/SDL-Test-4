#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <nlohmann/json.hpp>

#include "vect.h"

class Window 
{
public:
	Window(const nlohmann::json& constants);
	Window();
	~Window();

	void update();
	void events();

	// Rendering
	void drawRect(const Vect<int32_t> pos, const Vect<int32_t> size, const std::vector<uint8_t> color);

	const bool isClosed() const { return quit; }
	
	// Get inputs
	const Vect<int> getMousePos() const { return mousePos; }

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vect<uint32_t> winSize;
	
	bool quit;

	Vect<int> mousePos;
};