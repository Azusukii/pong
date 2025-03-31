#pragma once

namespace Constants {
	//windows
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	// paddles
	const int PADDLE_WIDTH = 20;
	const int PADDLE_HEIGHT = 100;
	const int PADDLE_SPEED = 5;
	// ball
	const int BALL_SIZE = 10;
	const int BALL_SPEED = 5;
	const int BALL_SPEED_X = 5;
	const int BALL_SPEED_Y = 5;
	// scores
	const int WINNING_SCORE = 99999;
	// positions
	const int LEFT_PADDLE_X = 10;
	const int RIGHT_PADDLE_X = SCREEN_WIDTH - 20;
	const int LEFT_PADDLE_Y = SCREEN_HEIGHT / 2 - 50;
	const int RIGHT_PADDLE_Y = SCREEN_HEIGHT / 2 - 50;
	const int BALL_X = SCREEN_WIDTH / 2 - 10;
	const int BALL_Y = SCREEN_HEIGHT / 2 - 10;
}
