#include "Ball.h"
#include <cmath>
#include <iostream>
#include "Constants.h"
#include "Game.h"

Ball::Ball(int x, int y, int w, int h, float sX, float sY) {
	rect = { x, y, w, h }; //init pos size
	speedX = sX;
	speedY = sY;
	std::cout << "Ball created at: (" << x << ", " << y << ")" << std::endl;
}

void Ball::updatePhysics() {
	rect.x += static_cast<int>(std::round(speedX)); // move the ball horizontally
	rect.y += static_cast<int>(std::round(speedY)); // move the ball vertically
	
	// handle wall collisions
	if (rect.y <= 0) { 
		rect.y = 0; 
		speedY = abs(speedY);
		std::cout << "Collision with top wall!" << std::endl;

		//play wall hit sfx
		if (Game::wallHitSound) {
			Mix_PlayChannel(-1, Game::wallHitSound, 0);
		}
	}
	else if (rect.y >= Constants::SCREEN_HEIGHT - rect.h) { 
		rect.y = Constants::SCREEN_HEIGHT - rect.h; 
		speedY = -abs(speedY);
		std::cout << "Collision with bottom wall!" << std::endl;

		if (Game::wallHitSound) {
			Mix_PlayChannel(-1, Game::wallHitSound, 0);
		}
	}
}

void Ball::paddleCollision(const SDL_Rect& leftPaddle, const SDL_Rect& rightPaddle) {
	//if hit paddles
	 if (SDL_HasIntersection(&rect, &leftPaddle)) {
		std::cout << "Collision with left paddle!" << std::endl;
		speedX = abs(speedX);
		const float speedIncreaseFactor = 1.1f; // increase speed by 10% on paddle hit
		const float maxSpeed = Constants::BALL_SPEED_X * 1.5f; // maximum speed increase of 50%
		if (speedX < maxSpeed) {
			speedX *= speedIncreaseFactor; // gradually increase speed
		}
		else {
			speedX = maxSpeed; // cap speed
		}
		//play paddle hit sfx
		if (Game::paddleHitSound) {
			Mix_PlayChannel(-1, Game::paddleHitSound, 0);
		}
	}
	else if (SDL_HasIntersection(&rect, &rightPaddle)) {
		std::cout << "Collision with right paddle!" << std::endl;
		speedX = -abs(speedX); 
		const float speedIncreaseFactor = 1.1f;
		const float maxSpeed = Constants::BALL_SPEED_X * 1.5f;
		if (abs(speedX) < maxSpeed) {
			speedX *= speedIncreaseFactor;
		}
		else {
			speedX = -maxSpeed;
		}
		//play paddle hit sfx
		if (Game::paddleHitSound) {
			Mix_PlayChannel(-1, Game::paddleHitSound, 0);
		}
	}
}

Ball::scoreResult Ball::checkScore() const {
	// std::cout << "Checking for score..." << std::endl;
	if (rect.x <= 0) { // updated to <= to catch zero. apparently worked cause its counting scores now
		// play score sfx
		if (Game::scoreSound) {
			Mix_PlayChannel(-1, Game::scoreSound, 0);
		}
		std::cout << "Right scored!" << std::endl;
		return scoreResult::right_scored;
	}
	if (rect.x >= Constants::SCREEN_WIDTH - rect.w) { // change to using constants for versatility
		if (Game::scoreSound) {
			Mix_PlayChannel(-1, Game::scoreSound, 0);
		}
		std::cout << "Left scored!" << std::endl;
		return scoreResult::left_scored;
	}
	return scoreResult::none;
}

void Ball::resetPos(int x, int y) {
	rect.x = x;
	rect.y = y;
	std::cout << "Resetting ball position to: (" << x << ", " << y << ")" << std::endl;
	speedX = -speedX; // reverse direction after reset (for fairness)
}

void Ball::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color
	SDL_RenderFillRect(renderer, &rect); // drawing the ball
}