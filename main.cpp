#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Game.h"

int main(int argc, char* argv[]) {
	Game game;
	if (game.init()) {
		game.run();
	}
	return 0;
}