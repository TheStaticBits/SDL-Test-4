#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "utility/vect.h"

class Window
{
public:
	Window(const nlohmann::json& constants);
	void destroy();

	void init(const nlohmann::json& constants);

	void update();
	void updateDeltaTime();
	void events();

	inline const bool isClosed() const { return exit; }

	[[nodiscard]] SDL_Texture* loadTexture(std::string path);

	void setTarget(SDL_Texture* texture);
	void resetTarget();

	void setBlendMode(SDL_BlendMode blendMode);
	void resetBlendMode();

	void drawRect(const SDL_Rect& rect, const std::vector<uint8_t>& color);
	void render(SDL_Texture* texture, const SDL_Rect& dest);
	void render(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest);

	inline const bool getKeyState(SDL_Keycode key) const { return keyStates.at(key); }
	inline const bool keyJustPressed(SDL_Keycode key) const { return keyStates.at(key) && !lastFrameKeys.at(key); }
	inline const float getDeltaTime() const { return deltaTime; }

private:
	void handleKey(std::unordered_map<SDL_Keycode, bool>& keyMap, SDL_Keycode key, uint32_t event);

	SDL_Window* window;
	SDL_Renderer* renderer;

	Vect<uint32_t> size;
	bool exit;
	
	float deltaTime;
	std::chrono::high_resolution_clock::time_point lastTime;

	std::unordered_map<SDL_Keycode, bool> keyStates; // The current frame's keys
	std::unordered_map<SDL_Keycode, bool> lastFrameKeys; // The previous frame's keys

	const std::vector<SDL_Keycode> allowedKeys = { SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN }; // only allowed keys to be detected
};