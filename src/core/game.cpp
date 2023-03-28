#include "core/game.h"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/save.h"
#include "core/window.h"
#include "helpers/textureHelpers.h"


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
}


void Game::iteration()
{
	window.update();
	window.events();
}


void Game::saveData()
{
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