#include "window.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vect.h"

Window::Window(const nlohmann::json& constants)
	: window(nullptr), renderer(nullptr), exit(false)
{
	init(constants);
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Window::init(const nlohmann::json& constants)
{
	// Retrieve window data from JSON constants
	const std::string title = constants["display"]["title"].get<std::string>();
	size = Vect<uint32_t>(constants["display"]["size"]);

	// Renderer flags
	int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	if (constants["display"]["vsync"]) flags |= SDL_RENDERER_PRESENTVSYNC; // Vsync

	// Creating window and renderer
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, flags);
}

void Window::update()
{
	SDL_RenderPresent(renderer); // Show frame
	SDL_RenderClear(renderer); // Clear window
}

void Window::events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			exit = true; break;
		}
	}
}

void Window::drawRect(const Vect<int32_t>&& pos, const Vect<uint32_t>&& size, const std::vector<uint8_t>&& color)
{
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]); // Set draw color

	const SDL_Rect rect = { pos.x, pos.y, size.xCast<int32_t>(), size.yCast<int32_t>() };
	if (SDL_RenderFillRect(renderer, &rect) != 0) // Draw rect
		std::cout << "Failed to draw rect" << SDL_GetError() << std::endl;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color
}