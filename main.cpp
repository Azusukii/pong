#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "paddle.h"
#include "ball.h"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init messed up! Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// window dimensions
	const int windowWidth = 800;
	const int windowHeight = 600;


	SDL_Window* window = SDL_CreateWindow("ponggers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!window) {
		std::cerr << "SDL_CreateWindow messed up! Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "SDL_CreateRenderer messed up! Error: " << SDL_GetError() << std::endl;
		return 1;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// create paddles
	Paddle leftPaddle(10, windowHeight / 2 - 50, 10, 100, 5, windowHeight); 
	Paddle rightPaddle(windowWidth - 20, windowHeight / 2 - 50, 10, 100, 5, windowHeight);

	// create ball
	Ball ball(windowWidth / 2 - 10, windowHeight / 2 - 10, 20, 20, 5, 5); //should be center of screen

	bool running = true;


	while(running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		// keyboard inputs for the paddles
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

		// update ball
		ball.Update(leftPaddle.rect, rightPaddle.rect);


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// draw paddles
		leftPaddle.render(renderer);
		rightPaddle.render(renderer);

		// draw ball
		ball.Render(renderer);


		SDL_RenderPresent(renderer);

		SDL_Delay(16); // roughly 60FPS?
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	return 0;
}