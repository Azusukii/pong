#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
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

	//sfx
	static Mix_Chunk* paddleHitSound;
	static Mix_Chunk* scoreSound;
	static Mix_Chunk* wallHitSound;
	static Mix_Chunk* titleSound;
	static Mix_Chunk* stageStartSound;
	static Mix_Chunk* gameOverSound;

private: 
	SDL_Window* window;
	SDL_Renderer* SDL_renderer;

	//game objects
	Paddle leftPaddle;
	Paddle rightPaddle;
	Ball ball;
	ScoreManager scoreManager;
	Renderer* renderer;
	InputHandler input;
	TextRenderer textRenderer;

	// game state
	bool running;
	GameState::State currentState;
	
	// score message
	bool showScoreMessage;
	std::string scoreMessage;
	Uint32 messageStartTime;

	// winner text (game over basically)
	std::string winnerText;

	// stage start variables
	bool isStageStarting;
	Uint32 stageStartTime;

	void resetGame();
	void handleStateTransitions(GameState::State lastState);
};
