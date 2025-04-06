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
	running(false),
	showScoreMessage(false),
	scoreMessage(""),
	messageStartTime(0) { }

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
	
	if (TTF_Init() != 0) { // initialize SDL_ttf 
		std::cerr << "SDL_ttf initialization messed up! Error: " << TTF_GetError() << std::endl;
		return false;
	}

	if (!textRenderer.init("fonts/PressStart2P-vaV7.ttf", 28)) {
		std::cerr << "TextRenderer initialization messed up!" << std::endl;
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
	if (showScoreMessage) {
		// if message_display_time passsed -> hide message
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - messageStartTime >= Constants::MESSAGE_DISPLAY_TIME) {
			showScoreMessage = false;
		}
		return;
	}
	//std::cout << "Checking for paddle collision..." << std::endl;
	ball.paddleCollision(leftPaddle.rect, rightPaddle.rect);

	//std::cout << "Updating ball physics..." << std::endl;
	ball.updatePhysics();

	Ball::scoreResult result = ball.checkScore();
	//std::cout << "Checking for score..." << std::endl;

	if (result != Ball::scoreResult::none) {
		scoreManager.updateScore(result);

		// message text based on who scored
		if (result == Ball::scoreResult::left_scored) {
			scoreMessage = "Left Player Scored!";
		}
		else {
			scoreMessage = "Right Player Scored!";
		}

		// -> show message and start timer
		showScoreMessage = true;
		messageStartTime = SDL_GetTicks();


		ball.resetPos(Constants::BALL_X, Constants::BALL_Y);

		if (scoreManager.checkWin()) {
			running = false;
		}
	}
}

void Game::render() {
	renderer->clear();
	// std::cout << "Rendering game objects..." << std::endl;
	renderer->render(ball, leftPaddle, rightPaddle);
	textRenderer.renderScore(SDL_renderer, scoreManager.getLeftScore(), scoreManager.getRightScore());
	// display score message in center
	if (showScoreMessage) {
		SDL_Color textColor = { 255, 255, 255, 255 };

		textRenderer.renderCenteredText(SDL_renderer, scoreMessage, Constants::MESSAGE_Y_POS, textColor);
	}
	renderer->present();
}

void Game::clean() {
	textRenderer.clean();
	delete renderer; // free the renderer memory
	SDL_DestroyWindow(window);
	TTF_Quit;
	SDL_Quit();
}