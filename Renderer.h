#pragma once
#include <SDL.h>
#include <iostream>
#include "Ball.h"
#include "Paddle.h"

class Renderer {
public:
	Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	void clear();
	void render(const Ball& ball, const Paddle& leftPaddle, const Paddle& rightPaddle);
	void present();
private:
	SDL_Renderer* renderer;
};
	