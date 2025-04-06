#pragma once

namespace Constants {
	//windows
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 760;
	// paddles
	const int PADDLE_WIDTH = 10;
	const int PADDLE_HEIGHT = 100;
	const int PADDLE_SPEED = 5;
	// ball
	const int BALL_SIZE = 20;
	const int BALL_SPEED = 5;
	const int BALL_SPEED_X = 5;
	const int BALL_SPEED_Y = 5;
	// scores
	const int WINNING_SCORE = 5;
	// positions
	const int LEFT_PADDLE_X = 10;
	const int RIGHT_PADDLE_X = SCREEN_WIDTH - 20;
	const int LEFT_PADDLE_Y = SCREEN_HEIGHT / 2 - 50;
	const int RIGHT_PADDLE_Y = SCREEN_HEIGHT / 2 - 50;
	const int BALL_X = SCREEN_WIDTH / 2 - 10;
	const int BALL_Y = SCREEN_HEIGHT / 2 - 10;
	//display message
	const int MESSAGE_DISPLAY_TIME = 3000; //3000ms approx 3 sec
	const int MESSAGE_Y_POS = SCREEN_HEIGHT / 2 - 50; // text pos middle of screen
}

namespace GameState {
	enum class State {
		TITLE,
		PLAYING,
		PAUSED,
		GAME_OVER
	};
}
