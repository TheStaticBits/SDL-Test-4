#include "game.h"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "window.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

Game::Game()
	: constants(nlohmann::json::parse(std::ifstream("data/constants.json"))), // Load constants JSON from file
	  window(constants), exitStatus(0)
{
	
}

Game::~Game()
{

}


void Game::iteration()
{
	window.update();
	window.events();

	window.drawRect({ 40, 40 }, { 40, 40 }, { 255, 255, 255, 255 });
}

#ifdef __EMSCRIPTEN__
void it(void* arg)
{
	Game* game = (Game*)arg;
	game->iteration();
}
#endif


void Game::mainLoop()
{
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(it, this, 0, 1);
#else
	while (!window.isClosed())
		iteration();
#endif
}