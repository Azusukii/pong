#pragma once
#include <SDL.h>
#include <iostream>
#include "Ball.h"

class ScoreManager {
public:
	ScoreManager(int winScore);
	~ScoreManager();

	void updateScore(Ball::scoreResult result);
	bool checkWin() const;
	int getLeftScore() const;
	int getRightScore() const;

	void displayScore() const;
private:
	int leftScore;
	int rightScore;
	const int winningScore;
};

