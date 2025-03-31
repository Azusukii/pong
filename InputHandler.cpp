#include "InputHandler.h"

bool InputHandler::handleEvents(bool& running) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
			return false;
		}
	}
	return true;
}

void InputHandler::handleInput(Paddle& leftPaddle, Paddle& rightPaddle) {
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);
	if (keystates[SDL_SCANCODE_W]) { // left up
		leftPaddle.movement(-1);
	}
	if (keystates[SDL_SCANCODE_S]) { // left down
		leftPaddle.movement(1);
	}
	if (keystates[SDL_SCANCODE_UP]) { // right up
		rightPaddle.movement(-1);
	}
	if (keystates[SDL_SCANCODE_DOWN]) { // right down
		rightPaddle.movement(1);
	}
}
