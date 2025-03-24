#include "paddle.h"

Paddle::Paddle(int x, int y, int w, int h, float s, int winH) {
	rect = { x, y, w, h }; //init pos size
	speed = s; // init speed
	windowHeight = winH; // init window height
}

// for the movement

void Paddle::movement(float deltaY) {
	rect.y += deltaY * speed; // move the paddle
	if (rect.y < 0) rect.y = 0; // boundary for top
	if (rect.y > windowHeight - rect.h) rect.y = windowHeight - rect.h; // boundary for bottom
}

void Paddle::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color
	SDL_RenderFillRect(renderer, &rect); // drawing the paddle
}