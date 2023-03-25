#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vect.h"

class Window
{
public:
	Window(const nlohmann::json& constants);
	~Window();

	void init(const nlohmann::json& constants);

	void update();
	void events();

	const bool isClosed() const { return exit; }

	void drawRect(const Vect<int32_t>&& pos, const Vect<uint32_t>&& size, const std::vector<uint8_t>&& color);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vect<uint32_t> size;
	bool exit;
};