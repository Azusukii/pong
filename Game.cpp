#include "Game.h"
#include <iostream>

//initialise sfx
Mix_Chunk* Game::paddleHitSound = nullptr;
Mix_Chunk* Game::scoreSound = nullptr;
Mix_Chunk* Game::wallHitSound = nullptr; // initialize sound effects
Mix_Chunk* Game::titleSound = nullptr;
Mix_Chunk* Game::stageStartSound = nullptr;
Mix_Chunk* Game::gameOverSound = nullptr;

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
	winnerText(""), // initialize winner text
	isStageStarting(false), // initialize stage start
	stageStartTime(0) {} // initialize stage start time
	


Game::~Game() {
	clean();
}

bool Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {// initialize SDL_mixer
		std::cerr << "SDL_mixer initialization messed up! Error: " << Mix_GetError() << std::endl;
		return false;
	}
	else {
		// load sound effects
		paddleHitSound = Mix_LoadWAV("sfx/paddle.wav");
		if (!paddleHitSound) {
			std::cerr << "Paddle hit sound loading messed up! Error: " << Mix_GetError() << std::endl;
			return false;
		}
		scoreSound = Mix_LoadWAV("sfx/score.wav");
		if (!scoreSound) {
			std::cerr << "Score sound loading messed up! Error: " << Mix_GetError() << std::endl;
			return false;
		}
		wallHitSound = Mix_LoadWAV("sfx/wall.wav");
		if (!wallHitSound) {
			std::cerr << "Wall hit sound loading messed up! Error: " << Mix_GetError() << std::endl;
			return false;
		}
		titleSound = Mix_LoadWAV("sfx/title.wav");
		if (!titleSound) {
			std::cerr << "Title sound loading messed up! Error: " << Mix_GetError() << std::endl;
		}
		stageStartSound = Mix_LoadWAV("sfx/stagestart.wav");
		if (!stageStartSound) {
			std::cerr << "Stage start sound loading messed up! Error: " << Mix_GetError() << std::endl;
		}
		gameOverSound = Mix_LoadWAV("sfx/gameover.wav");
		if (!gameOverSound) {
			std::cerr << "Game over sound loading messed up! Error: " << Mix_GetError() << std::endl;
		}
	}

	if (!textRenderer.init("fonts/PressStart2P-vaV7.ttf", "fonts/KarmaticArcade-6Yrp1.ttf", 28, 48)) {
		std::cerr << "TextRenderer initialization messed up!" << std::endl;
		return false;
	}
	
	renderer = new Renderer(SDL_renderer);
	std::cout << "Renderer initialized" << std::endl;
	
	running = true;

	// play title sound upon game start
	if (titleSound) {
		Mix_PlayChannel(-1, titleSound, 0);
	}
	return true;
}

void Game::run() {
	auto input = InputHandler();
	while (running) {
		input.handleEvents(running, currentState);
		input.handleInput(leftPaddle, rightPaddle, currentState);

		update();

		render();

		SDL_Delay(1000 / 60); // 60 FPS
	}
}

void Game::handleStateTransitions(GameState::State lastState) {
	if (lastState != currentState) {

		Mix_HaltChannel(-1); // stop all sounds before playing new one
		if (lastState == GameState::State::TITLE && currentState == GameState::State::PLAYING) {
			// redirect to stage start
			currentState = GameState::State::STAGE_START;
			isStageStarting = true;
			stageStartTime = SDL_GetTicks();

			// play stage start sound
			if (stageStartSound) {
				Mix_PlayChannel(-1, stageStartSound, 0);
			}
		}
		// game over to playing transition (through stage start)
		else if (lastState == GameState::State::GAME_OVER && currentState == GameState::State::PLAYING) {
			resetGame();
			currentState = GameState::State::STAGE_START;
			isStageStarting = true;
			stageStartTime = SDL_GetTicks();

			// play stage start sound
			if (stageStartSound) {
				Mix_PlayChannel(-1, stageStartSound, 0);
			}
		}
		// pause to title transition
		else if (lastState == GameState::State::PAUSED && currentState == GameState::State::TITLE) {
			resetGame();

			// play title sound
			if (titleSound) {
				Mix_PlayChannel(-1, titleSound, 0);
			}
		}
		else if (lastState != GameState::State::GAME_OVER && currentState == GameState::State::GAME_OVER) {
			// play game over sound
			if (gameOverSound) {
				Mix_PlayChannel(-1, gameOverSound, 0);
			}
		}
	}
}
void Game::update() {
	// checking for transitions that require reset
	static GameState::State lastState = currentState;
	// handle state transitions and sounds
	handleStateTransitions(lastState);

	// stage start timer
	if (currentState == GameState::State::STAGE_START) {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - stageStartTime >= Constants::STAGE_START_TIME) {
			currentState = GameState::State::PLAYING;
			isStageStarting = false;
		}
		lastState = currentState; // update last state
		return; // don't process anything while in stage start
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
		// reverse direction for fairness but keep initial speed
		ball.speedX = -ball.speedX > 0 ? Constants::BALL_SPEED_X : -Constants::BALL_SPEED_X;

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
		textRenderer.renderTitleCenteredText(SDL_renderer, "PONGGERS", Constants::SCREEN_HEIGHT / 3, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Press SPACE to start!", Constants::SCREEN_HEIGHT / 2, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "A small game by Azura", Constants::SCREEN_HEIGHT / 1.5, { 255, 255, 255, 255 });
		break;
	case GameState::State::STAGE_START:
		textRenderer.renderCenteredText(SDL_renderer, "GET READY!", Constants::SCREEN_HEIGHT / 3, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Stage Starting...", Constants::SCREEN_HEIGHT / 2, { 255, 255, 255, 255 });
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
		textRenderer.renderCenteredText(SDL_renderer, "Press SPACE to reset", Constants::SCREEN_HEIGHT / 2 + 50, { 255, 255, 255, 255 });
		textRenderer.renderCenteredText(SDL_renderer, "Press ESC to unpause", Constants::SCREEN_HEIGHT / 2 + 100, { 255, 255, 255, 255 });
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
	ball.resetSpeed(Constants::BALL_SPEED_X, Constants::BALL_SPEED_Y);

	// reset score
	scoreManager.reset();

	// reset variables
	showScoreMessage = false;
	scoreMessage = "";
	winnerText = "";
	isStageStarting = false;
}

void Game::clean() {
	// free sfx
	if (paddleHitSound) {
		Mix_FreeChunk(paddleHitSound);
		paddleHitSound = nullptr;
	}
	if (scoreSound) {
		Mix_FreeChunk(scoreSound);
		scoreSound = nullptr;
	}
	if (wallHitSound) {
		Mix_FreeChunk(wallHitSound);
		wallHitSound = nullptr;
	}
	if (titleSound) {
		Mix_FreeChunk(titleSound);
		titleSound = nullptr;
	}
	if (stageStartSound) {
		Mix_FreeChunk(stageStartSound);
		stageStartSound = nullptr;
	}
	if (gameOverSound) {
		Mix_FreeChunk(gameOverSound);
		gameOverSound = nullptr;
	}
	Mix_CloseAudio();
	textRenderer.clean();
	delete renderer; // free the renderer memory
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}