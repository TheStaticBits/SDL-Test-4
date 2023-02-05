#include "window.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

Window::Window(const nlohmann::json& constants)
	: window(NULL), renderer(NULL), 
	  winSize(constants["display"]["size"]),
	  quit(false)
{
	std::string title = constants["display"]["title"].get<std::string>();

	// Creating window, centered on the screen
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winSize.x, winSize.y, SDL_WINDOW_SHOWN);
	
	if (window == NULL)
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
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
			quit = true; break;
		}
	}
}

void Window::drawRect(const Vect<int32_t> pos, const Vect<int32_t> size, const std::vector<uint8_t> color)
{
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]); // Set draw color
	
	const SDL_Rect rect = { pos.x, pos.y, size.x, size.y };
	if (SDL_RenderFillRect(renderer, &rect) != 0) // Draw rect
		std::cout << "Failed to draw rect" << SDL_GetError() << std::endl;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color
}