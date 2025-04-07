#include "InputHandler.h"

bool InputHandler::handleEvents(bool& running, GameState::State& currentState) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
			return false;
		}
		// key presses for state changes
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_SPACE: // space bar for main control key for state transition
				if (currentState == GameState::State::TITLE) {
					currentState = GameState::State::PLAYING;
				}
				else if (currentState == GameState::State::GAME_OVER) {
					currentState = GameState::State::PLAYING;
				}
				else if (currentState == GameState::State::PAUSED) {
					currentState = GameState::State::PLAYING;
				}
				break;

			case SDLK_ESCAPE: // escape for pausing and unpausing
				if (currentState == GameState::State::PLAYING) {
					currentState = GameState::State::PAUSED;
				}
				else if (currentState == GameState::State::PAUSED) {
					currentState = GameState::State::PLAYING;
				}
				break;
			}
		}
	}
	return true;
}

void InputHandler::handleInput(Paddle& leftPaddle, Paddle& rightPaddle, GameState::State currentState) {
	// only process if game is in playing state
	if (currentState != GameState::State::PLAYING) {
		return;
	}
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
