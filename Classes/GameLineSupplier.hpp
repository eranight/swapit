#pragma once

#include "LineSupplier.hpp"
#include "LevelProbabilities.hpp"
#include <random>

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