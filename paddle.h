#pragma once
#include <SDL.h>

class Paddle {
public: 
	SDL_Rect rect; // for the pos and size
	float speed;   // for the speed (duh)
	int windowHeight; // for the window height

	Paddle(int x, int y, int w, int h, float s, int winH); //constructor (note to self does not have the form with {} at the end)
		void movement(float deltaY); // for the movement
		void render(SDL_Renderer * renderer); // for rendering
};