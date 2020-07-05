#pragma once

#include "LineSupplier.hpp"
#include <random>

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

class GameLineSupplier : public LineSupplier {
public:
	GameLineSupplier(const LineInfo & initialLine, std::vector<LevelProbabilities> levels = {});
	LineInfo getNextLine() override;
	void reset(const LineInfo & initialLine);
private:
	enum class Action { stand = 0, swap, middle };

	enum class Amount { one = 0, two };

	enum class Wall { yes = 0, no };

	class LineDescription {
	public:
		Action action;
		Amount amount;
		Wall wall;
		int mask[3];
		int position;
	};

private:
	int castDice(std::vector<const int> events);
	Action generateAction(int mask[], int & postition);
	Amount generateAmount(int mask[]);
	Wall generateWall(int mask[], Action action);
private:
	std::mt19937_64 mt;
	std::uniform_int_distribution<int> generator;
	LineInfo initialLine;
	LineDescription line;
	std::vector<LevelProbabilities> levels;
	std::vector<LevelProbabilities>::iterator currentLevel;
	int leftside, rightside;
};