#pragma once
#include <SDL.h>
#include "Paddle.h"
#include "Constants.h"

class InputHandler {
public:
	InputHandler() = default;
	~InputHandler() = default;
	bool handleEvents(bool& running, GameState::State& currentState);
	void handleInput(Paddle& leftPaddle, Paddle& rightPaddle, GameState::State currentState);
};
