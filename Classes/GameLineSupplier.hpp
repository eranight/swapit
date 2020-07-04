#pragma once

#include "LineSupplier.hpp"
#include <random>

class GameLineSupplier : public LineSupplier {
public:
	GameLineSupplier();
	LineInfo getNextLine() override;
	void reset() { isFistLine = true; }
private:
	enum class Action { stand, swap, middle };

	enum class Amount { one, two };

	enum class Wall { yes, no };

	class LineDescription {
	public:
		const Action action;
		const Amount amount;
		const Wall wall;
	};

private:
	int castDice(std::initializer_list<int> events);
private:
	std::mt19937_64 mt;
	std::uniform_int_distribution<int> generator;

	bool isFistLine;
	LineDescription line;
};