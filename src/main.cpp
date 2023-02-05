#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"

int main(int argc, char* args[])
{
	Game game;
	
	game.mainLoop();

	return game.getExitStatus();
}