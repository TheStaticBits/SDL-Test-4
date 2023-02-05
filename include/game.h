#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "window.h"

class Game
{
public:
	Game();
	~Game();

	void mainLoop();

	const int getExitStatus() const { return exitStatus; }

private:
	const nlohmann::json constants;
	
	Window window;

	int exitStatus;
};