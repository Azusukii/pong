#include "ball.h"
#include <cmath>

Ball::Ball(int x, int y, int w, int h, float sX, float sY) {
	rect = { x, y, w, h }; //init pos size
	speedX = sX; // init speedX
	speedY = sY; // init speedY
}

void Ball::Update(const SDL_Rect& leftPaddle, const SDL_Rect& rightPaddle) {
	rect.x += static_cast<int>(std::round(speedX)); // move the ball up    //updated to round for int float error
	rect.y += static_cast<int>(std::round(speedY)); // move the ball down

	if (rect.y < 0 || rect.y > 600 - rect.h) { // if the ball hits the top or bottom
		speedY = -speedY; // reverse the speed
	}

	if (SDL_HasIntersection(&rect, &leftPaddle)) {
		speedX = abs(speedX); // reverse the speed
	}
	if (SDL_HasIntersection(&rect, &rightPaddle)) {
		speedX = -abs(speedX); // reverse the speed
	}

	if (rect.x < 0 || rect.x > 800 - rect.w) { // if the ball hits the left or right
		rect.x = 800 / 2 - rect.w / 2; // reset the ball pos
		rect.y = 600 / 2 - rect.h / 2; // reset the ball pos
		speedX = -speedX; // reverse the speed
	}
}

void Ball::ResetPos(int x, int y) {
	rect.x = x; // reset pos x
	rect.y = y; // reset pos y
}

void Ball::Render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color
	SDL_RenderFillRect(renderer, &rect); // drawing the ball
}