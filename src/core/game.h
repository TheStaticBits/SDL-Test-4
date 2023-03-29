#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/save.h"
#include "core/window.h"

class Game
{
public:
	Game();
	~Game();

	static void initSDL();

	void initObjects();

	void iteration();
	void saveData();
	
	void start();

private:
	const nlohmann::json constants;
	entt::registry registry;

	Save save;
	Window window;
};