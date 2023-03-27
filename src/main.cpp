#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>

#include "core/game.h"

int main(int argc, char* argv[])
{
	Game::initSDL();
	Game game;
	game.start();

	return 0;
}