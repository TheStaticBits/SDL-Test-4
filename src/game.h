#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "save.h"
#include "window.h"

class Game
{
public:
	Game();

	static void initSDL();

	void iteration();
	void saveData();
	
	void start();

private:
	const nlohmann::json constants;
	entt::registry registry;

	Save save;
	Window window;
};