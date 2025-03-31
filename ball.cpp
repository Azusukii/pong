#include "Ball.h"
#include <cmath>
#include <iostream>
#include "Constants.h"

Ball::Ball(int x, int y, int w, int h, float sX, float sY) {
	rect = { x, y, w, h }; //init pos size
	speedX = sX; // init speedX
	speedY = sY; // init speedY
	std::cout << "Ball created at: (" << x << ", " << y << ")" << std::endl;
}

void Ball::updatePhysics() {
	rect.x += static_cast<int>(std::round(speedX)); // move the ball horizontally
	rect.y += static_cast<int>(std::round(speedY)); // move the ball vertically
	
	// handle wall collisions
	if (rect.y <= 0) { 
		rect.y = 0; // prevent ball from going outside the screen
		speedY = std::abs(speedY);
		std::cout << "Collision with top wall!" << std::endl;
	}
	else if (rect.y >= Constants::SCREEN_HEIGHT - rect.h) { 
		rect.y = Constants::SCREEN_HEIGHT - rect.h; // same as above
		speedY = -std::abs(speedY);
		std::cout << "Collision with bottom wall!" << std::endl;
	}
}

void Ball::paddleCollision(const SDL_Rect& leftPaddle, const SDL_Rect& rightPaddle) {
	//if hit paddles
	 if (SDL_HasIntersection(&rect, &leftPaddle)) {
		std::cout << "Collision with left paddle!" << std::endl;
		speedX = abs(speedX); // reverse the speed
	}
	else if (SDL_HasIntersection(&rect, &rightPaddle)) {
		std::cout << "Collision with right paddle!" << std::endl;
		speedX = -abs(speedX); // reverse the speed
	}
}

bool Ball::outOfBounds() const {
	return (rect.x <= 0 || rect.x >= Constants::SCREEN_WIDTH - rect.w);
}

Ball::scoreResult Ball::checkScore() const {
	std::cout << "Checking for score..." << std::endl;
	if (rect.x <= 0) { // updated to <= to catch zero. apparently worked cause its counting scores now
		std::cout << "Right scored!" << std::endl;
		std::cout.flush();
		return scoreResult::right_scored; // right scored
	}
	if (rect.x >= Constants::SCREEN_WIDTH - rect.w) { // change to using constants for versatility
		std::cout << "Left scored!" << std::endl;
		std::cout.flush();
		return scoreResult::left_scored; // left scored
	}
	return scoreResult::none; // no score
}

void Ball::resetPos(int x, int y) {
	rect.x = x; // reset pos x
	rect.y = y; // reset pos y
	std::cout << "Resetting ball position to: (" << x << ", " << y << ")" << std::endl;
	std::cout.flush();
	speedX = -speedX; // reverse direction after reset (for fairness)
}

void Ball::Render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color
	SDL_RenderFillRect(renderer, &rect); // drawing the ball
}