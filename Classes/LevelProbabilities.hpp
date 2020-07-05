#pragma once

#include <vector>

class LevelProbabilities {
public:
	LevelProbabilities & operator=(const LevelProbabilities & other) {
		action = other.action;
		amount = other.amount;
		wall = other.wall;
		forceSwap = other.forceSwap;
		forceMiddle = other.forceMiddle;
		forceDouble = other.forceDouble;
		forceWall = other.forceWall;
		return *this;
	}
public:
	std::vector<const int> action;
	std::vector<const int> amount;
	int wall;
	bool forceSwap;
	bool forceMiddle;
	bool forceDouble;
	bool forceWall;
};