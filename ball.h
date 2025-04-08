#pragma once
#include <SDL.h>

class Ball {
public:
	enum class scoreResult { none, left_scored, right_scored };
	SDL_Rect rect; // for the pos and size
	float speedX, speedY;   // for the speed 

	Ball(int x, int y, int w, int h, float sX, float sY);
	void updatePhysics(); // update physics with wall collision															
	void paddleCollision(const SDL_Rect& leftPaddle, const SDL_Rect& rightPaddle); // check for paddle collision
	scoreResult checkScore() const; // check if a score has been made
	void resetPos(int x, int y); // reset ball pos
	void render(SDL_Renderer* renderer) const; // for rendering
	void resetSpeed(float sX, float sY);
};
