#include "Game.h"
#include <iostream>

// constructor
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
	messageStartTime(0),
	currentState(GameState::State::TITLE), // initialise with title state
	winnerText("") {}// initialize winner text


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
		input.handleEvents(running, currentState);
		input.handleInput(leftPaddle, rightPaddle, currentState);

		//update game
		update();

		//render game
		render();

		//delay -> 60fps
		SDL_Delay(1000/60);
	}
}

void Game::update() {
	// checking for transitions that require reset
	static GameState::State lastState = currentState;
	if (lastState == GameState::State::GAME_OVER && currentState == GameState::State::TITLE) {
		resetGame();
	}
	lastState = currentState;

	// handle different game states
	switch (currentState) {
	case GameState::State::TITLE: 

		return;

	case GameState::State::PAUSED:

		return;

	case GameState::State::GAME_OVER:

		return;

	case GameState::State::PLAYING:

		break;
	}
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
			// update winner text
			if (scoreManager.getLeftScore() >= Constants::WINNING_SCORE) {
				winnerText = "Left Player Wins!";
			}
			else {
				winnerText = "Right Player Wins!";
			}

			// game over state
			currentState = GameState::State::GAME_OVER;
		}
	}
}

void Game::render() {
	renderer->clear();
	// std::cout << "Rendering game objects..." << std::endl;

	// update render based on state
	switch (currentState) {
	case GameState::State::TITLE:
		textRenderer.renderCenteredText(SDL_renderer, "PONGGERS", Constants::SCREEN_HEIGHT / 3, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Press SPACE to start!", Constants::SCREEN_HEIGHT / 2, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "A small game by Azura", Constants::SCREEN_HEIGHT / 1.5, { 255, 255, 255, 255 });
		break;
	case GameState::State::PLAYING:
		renderer->render(ball, leftPaddle, rightPaddle);
		textRenderer.renderScore(SDL_renderer, scoreManager.getLeftScore(), scoreManager.getRightScore());
		// display score message in center
		if (showScoreMessage) {
			SDL_Color textColor = { 255, 255, 255, 255 };
			textRenderer.renderCenteredText(SDL_renderer, scoreMessage, Constants::MESSAGE_Y_POS, textColor);
		}
		break;

	case GameState::State::PAUSED: // should still show game but now with a pause message
		renderer->render(ball, leftPaddle, rightPaddle);
		textRenderer.renderScore(SDL_renderer, scoreManager.getLeftScore(), scoreManager.getRightScore());
		textRenderer.renderCenteredText(SDL_renderer, "PAUSED", Constants::SCREEN_HEIGHT / 2, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Press SPACE to continue", Constants::SCREEN_HEIGHT / 2 + 50, { 255, 255, 255, 255 });
		break;

	case GameState::State::GAME_OVER:
		textRenderer.renderCenteredText(SDL_renderer, "GAME OVER!", Constants::SCREEN_HEIGHT / 3, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, winnerText, Constants::SCREEN_HEIGHT / 2, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Press SPACE to restart", Constants::SCREEN_HEIGHT * 2 / 3, { 255, 255, 255, 255 });
		break;
	}
	renderer->present();
}

void Game::resetGame() {
	// reset paddles
	leftPaddle = Paddle(Constants::LEFT_PADDLE_X, Constants::LEFT_PADDLE_Y, Constants::PADDLE_WIDTH, Constants::PADDLE_HEIGHT, Constants::PADDLE_SPEED, Constants::SCREEN_HEIGHT);
	rightPaddle = Paddle(Constants::RIGHT_PADDLE_X, Constants::RIGHT_PADDLE_Y, Constants::PADDLE_WIDTH, Constants::PADDLE_HEIGHT, Constants::PADDLE_SPEED, Constants::SCREEN_HEIGHT);

	// reset ball
	ball = Ball(Constants::BALL_X, Constants::BALL_Y, Constants::BALL_SIZE, Constants::BALL_SIZE, Constants::BALL_SPEED_X, Constants::BALL_SPEED_Y);

	// reset score
	scoreManager.reset();

	// reset variables
	showScoreMessage = false;
	scoreMessage = "";
	winnerText = "";
}

void Game::clean() {
	textRenderer.clean();
	delete renderer; // free the renderer memory
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}