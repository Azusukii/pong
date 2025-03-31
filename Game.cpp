#include "Game.h"
#include <iostream>

Game::Game() :
	window(nullptr),
	SDL_renderer(nullptr),
	leftPaddle(Constants::LEFT_PADDLE_X, Constants::LEFT_PADDLE_Y, Constants::PADDLE_WIDTH, Constants::PADDLE_HEIGHT, Constants::PADDLE_SPEED, Constants::SCREEN_HEIGHT),
	rightPaddle(Constants::RIGHT_PADDLE_X, Constants::RIGHT_PADDLE_Y, Constants::PADDLE_WIDTH, Constants::PADDLE_HEIGHT, Constants::PADDLE_SPEED, Constants::SCREEN_HEIGHT),
	ball(Constants::BALL_X, Constants::BALL_Y, Constants::BALL_SIZE, Constants::BALL_SIZE, Constants::BALL_SPEED_X, Constants::BALL_SPEED_Y),
	scoreManager(Constants::WINNING_SCORE),
	renderer(nullptr),
	running(false) {
}

Game::~Game() {
	clean();
}

bool Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init messed up! Error: " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("ponggers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "SDL_CreateWindow messed up! Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!SDL_renderer) {
		std::cerr << "SDL_CreateRenderer messed up! Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	// new renderer rather than direct 
	renderer = new Renderer(SDL_renderer);
	std::cout << "Renderer initialized" << std::endl;
	
	running = true;
	return true;
}

void Game::run() {
	auto input = InputHandler();
	while (running) {
		//process inputs
		input.handleEvents(running);
		input.handleInput(leftPaddle, rightPaddle);

		//update game
		update();

		//render game
		render();

		//delay -> 60fps
		SDL_Delay(1000/60);
	}
}

void Game::update() {
	std::cout << "Checking for paddle collision..." << std::endl;
	ball.paddleCollision(leftPaddle.rect, rightPaddle.rect);

	std::cout << "Updating ball physics..." << std::endl;
	ball.updatePhysics();

	Ball::scoreResult result = ball.checkScore();
	std::cout << "Checking for score..." << std::endl;

	if (result != Ball::scoreResult::none) {
		scoreManager.updateScore(result);
		ball.resetPos(Constants::BALL_X, Constants::BALL_Y);

		if (scoreManager.checkWin()) {
			running = false;
		}
	}
}

void Game::render() {
	renderer->clear();
	std::cout << "Rendering game objects..." << std::endl;
	renderer->render(ball, leftPaddle, rightPaddle);
	renderer->present();
}

void Game::clean() {
	delete renderer; // free the renderer memory
	SDL_DestroyWindow(window);
	SDL_Quit();
}