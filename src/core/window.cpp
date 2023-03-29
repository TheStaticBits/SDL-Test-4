#include "core/window.h"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "utility/vect.h"

#define SDL_ERR(message) // std::cout << "Failed to " << message << ": " << SDL_GetError() << std::endl;
#define SDL_RUN(x, message) if (x != 0) { SDL_ERR(message); }

Window::Window(const nlohmann::json& constants)
	: window(nullptr), renderer(nullptr), exit(false), deltaTime(0), lastTime(0)
{
	init(constants);

	// Populate keyStates
	for (const SDL_Keycode key : allowedKeys)
	{
		keyStates[key] = false;
		lastFrameKeys[key] = false;
	}
}

void Window::destroy()
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

void Window::updateDeltaTime()
{
	deltaTime = static_cast<float>(SDL_GetTicks() - lastTime) / 1000.0f; // Delta time in seconds
	lastTime = SDL_GetTicks();
}

void Window::events()
{
	SDL_Event event;

	std::unordered_map<SDL_Keycode, bool> tempKeys = keyStates; // Copy current keys to temp

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) 
			exit = true;
		else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			handleKey(tempKeys, event.key.keysym.sym, event.type);
	}
	
	lastFrameKeys = keyStates; // Update last frame keys
	keyStates = tempKeys; // Update current keys
}

void Window::handleKey(std::unordered_map<SDL_Keycode, bool>& keyMap, SDL_Keycode key, uint32_t event)
{
	if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end()) // Check if key is allowed
		keyMap[key] = (event == SDL_KEYDOWN);
}

[[nodiscard]] SDL_Texture* Window::loadTexture(std::string path)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == nullptr) SDL_ERR("load image at " + path);

	return texture;
}


void Window::setTarget(SDL_Texture* texture)
{
	SDL_SetRenderTarget(renderer, texture);
}

void Window::resetTarget()
{
	SDL_SetRenderTarget(renderer, nullptr);
}

void Window::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(renderer, blendMode);
}

void Window::resetBlendMode()
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}


void Window::drawRect(const SDL_Rect& rect, const std::vector<uint8_t>& color)
{
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]); // Set draw color
	SDL_RUN(SDL_RenderFillRect(renderer, &rect), "draw rect") // Draw rect
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset color
}

void Window::render(SDL_Texture* texture, const SDL_Rect& dest)
{
	SDL_RUN(SDL_RenderCopy(renderer, texture, nullptr, &dest), "draw image without src");
}

void Window::render(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest)
{
	SDL_RUN(SDL_RenderCopy(renderer, texture, &src, &dest), "draw image with src");
}