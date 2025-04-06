#pragma once
#include <SDL.h>
#include <iostream>
#include "Ball.h"
#include "Constants.h"

class ScoreManager {
public:
	ScoreManager(int winScore);
	~ScoreManager();
	void updateScore(Ball::scoreResult result);
	bool checkWin() const;
	int getLeftScore() const;
	int getRightScore() const;
	void displayScore() const;
	void reset();
private:
	int leftScore;
	int rightScore;
	const int winningScore;
};

