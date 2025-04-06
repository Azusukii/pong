#include "ScoreManager.h"
#include "Constants.h"

ScoreManager::ScoreManager(int winScore) : leftScore(0), rightScore(0), winningScore(winScore) {}
ScoreManager::~ScoreManager() {}

void ScoreManager::updateScore(Ball::scoreResult result) {
    std::cout << "Before Update - Left: " << leftScore << " | Right: " << rightScore << std::endl;

    if (result == Ball::scoreResult::left_scored) {
        leftScore++;
    }
    else if (result == Ball::scoreResult::right_scored) {
        rightScore++;
    }

    std::cout << "After Update - Left: " << leftScore << " | Right: " << rightScore << std::endl;
}

bool ScoreManager::checkWin() const {
	return (leftScore >= winningScore || rightScore >= winningScore);
}

int ScoreManager::getLeftScore() const {
	return leftScore;
}

int ScoreManager::getRightScore() const {
	return rightScore;
}

void ScoreManager::displayScore() const {
	std::cout << "Left: " << leftScore << " | Right: " << rightScore << std::endl;
}
void ScoreManager::reset() {
	leftScore = 0;
	rightScore = 0;
	std::cout << "Scores reset to Left: " << leftScore << " | Right: " << rightScore << std::endl;
}