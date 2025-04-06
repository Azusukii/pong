#pragma once
#include <SDL.h>
#include <iostream>
#include "Ball.h"
#include "Paddle.h"
#include "ScoreManager.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Constants.h"
#include "TextRenderer.h"

class Game {
public: 
	Game();
	~Game();

	bool init();
	void run();
	void update();
	void render();
	void clean();
private: 
	SDL_Window* window;
	SDL_Renderer* SDL_renderer;
	Paddle leftPaddle;
	Paddle rightPaddle;
	Ball ball;
	ScoreManager scoreManager;
	Renderer* renderer;
	InputHandler input;
	bool running;
	TextRenderer textRenderer;
	bool showScoreMessage;
	std::string scoreMessage;
	Uint32 messageStartTime;
};
