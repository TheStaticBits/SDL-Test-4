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

	inline const bool isClosed() const { return exit; }

	SDL_Texture* loadTexture(std::string path);

	void setTarget(SDL_Texture* texture);
	void resetTarget();

	void setBlendMode(SDL_BlendMode blendMode);
	void resetBlendMode();

	void drawRect(const SDL_Rect& rect, const std::vector<uint8_t>& color);
	void render(SDL_Texture* texture, const SDL_Rect& dest);
	void render(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest);


private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vect<uint32_t> size;
	bool exit;
};