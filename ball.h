#pragma once
#include <SDL.h>

class Ball {
public: 
	SDL_Rect rect; // for the pos and size
	float speedX, speedY;   // for the speed 

	Ball(int x, int y, int w, int h, float sX, float sY);
	void Update(const SDL_Rect& leftPaddle, const SDL_Rect& rightPaddle); // update ball pos
	void ResetPos(int x, int y); // reset ball pos
	void Render(SDL_Renderer* renderer); // for rendering
};
