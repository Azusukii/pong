#pragma once
#include <SDL.h>
#include "Paddle.h"

class InputHandler {
public:
	InputHandler() = default;
	~InputHandler() = default;
	bool handleEvents(bool& running);
	void handleInput(Paddle& leftPaddle, Paddle& rightPaddle);
};
