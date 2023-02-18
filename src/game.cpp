#include "game.h"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "window.h"

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
	game->save();
	return "EEEE";
}
#endif


Game::Game()
	: constants(nlohmann::json::parse(std::ifstream("data/constants.json"))), // Load constants JSON from file
	  window(constants), saveData(constants),
	  exitStatus(0)
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

	window.drawRect(window.getMousePos().cast<int32_t>(), { 40, 40 }, { 255, 0, 0, 255 });
}


void Game::mainLoop()
{
#ifdef __EMSCRIPTEN__
	emscripten_set_beforeunload_callback(this, emscriptenSave);
	emscripten_set_main_loop_arg(emscriptenIteration, this, 0, 1);
#else
	while (!window.isClosed())
		iteration();
	save();
#endif

}


void Game::save()
{
	saveData.save(constants);
}