#include "core/game.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/save.h"
#include "core/window.h"
#include "core/factories.h"

#include "helpers/textureHelpers.h"
#include "helpers/playerHelpers.h"
#include "helpers/tileTexHelpers.h"
#include "helpers/mapGenHelpers.h"

#include "systems/moveSystem.h"
#include "systems/renderSystem.h"
#include "systems/cameraSystem.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include "emscripten/html5.h"
// Emscripten main loop, just calls the main loop of the Game object passed in
void emscriptenIteration(void* gamePtr)
{
	Game* game = (Game*)gamePtr;
	game->iteration();
}

// Emscripten beforeunload callback which calls the game save function
const char* emscriptenSave(int eventType, const void* reserved, void* gamePtr)
{
	Game* game = (Game*)gamePtr;
	game->saveData();
	return NULL;
}
#endif


Game::Game()
	: constants(nlohmann::json::parse(std::ifstream("data/constants.json"))),
	  registry(), save(constants), window(constants)
{

}

Game::~Game()
{
	Helpers::destroyTextures(registry);
	window.destroy();

	SDL_Quit();
	IMG_Quit();
}


void Game::initSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	srand(static_cast<uint32_t>(time(0)));
}

void Game::initObjects()
{
	Factories::makePlayer(registry, window, constants, save);
	Factories::makeCamera(registry, constants);

	Factories::makeTileTextureStorage(registry, window, constants);
	Factories::makeLayerGen(registry, constants);

	Helpers::generateTileTextures(registry, window, constants); // Creates all color-shifted tile textures
	Helpers::setupLayerGen(registry, constants);

	for (int i = 0; i < 10; i++)
	{
		Helpers::genLayer(registry, window, constants);
		Helpers::startNextLayerGen(registry);
	}
}


void Game::iteration()
{
	window.events();
	window.updateDeltaTime();

	Systems::updateKeyboardInputVels(registry, window);
	Systems::updateGravity(registry);
	Systems::updateVelocities(registry, window);
	Systems::capVelocities(registry);
	Systems::updateMovement(registry, window);
	Systems::updateMovWithCollisions(registry, window);

	Helpers::updatePlayerBoundaries(registry);

	Systems::updateCameraMovement(registry, window);
	Systems::render(registry, window);

	window.update();
}


void Game::saveData()
{
	Helpers::playerSave(registry, save);

	save.save(constants);
}


void Game::start()
{

#ifdef __EMSCRIPTEN__
	emscripten_set_beforeunload_callback(this, emscriptenSave);
	emscripten_set_main_loop_arg(emscriptenIteration, this, 0, 1);
#else
	while (!window.isClosed())
		iteration();
	saveData();
#endif

}