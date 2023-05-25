#pragma once

#include <iostream>
#include <unordered_map>

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

	std::unordered_map<std::string, entt::entity> singleEntities;

	Save save;
	Window window;
};