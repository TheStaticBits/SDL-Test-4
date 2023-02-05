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
		std::cout << "Failed to create window" << std::endl;

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