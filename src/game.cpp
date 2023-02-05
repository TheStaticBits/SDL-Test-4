#include "game.h"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

Game::Game()
	: constants(nlohmann::json::parse(std::ifstream("data/constants.json"))), // Load constants JSON from file
	  window(constants), exitStatus(0)
{
	
}

Game::~Game()
{

}

void Game::mainLoop()
{
	while (!window.isClosed())
	{
		window.update();
		window.events();
	}
}