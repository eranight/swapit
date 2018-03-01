#include "LineBuilder.h"
#include "cocos2d.h"

USING_NS_CC;

LineBuilder::LineBuilder() : lineCounter(0), sidesProb(0), wallsProb(0), swapProb(0), singleProb(0) {}

LineInfo LineBuilder::getNextLine() {
	LineInfo lineInfo;

	if (lineCounter == 0) {
		generateFirstLine(lineInfo);
	}
	else {
		if (prevLine.middle == LineInfo::Element::violet) {
			generateSwap(lineInfo);
			generateBallCount(lineInfo);
		}
		else {
			generateBalls(lineInfo);
		}
		generateWalls(lineInfo);
	}

	if (lineInfo.middle != LineInfo::Element::violet) {
		CCASSERT(lineInfo.left != LineInfo::Element::none || lineInfo.right != LineInfo::Element::none, "lol");
	}

	prevLine = lineInfo;
	++lineCounter;
	return lineInfo;
}

void LineBuilder::setProbabilities(int sides, int walls, int swap, int single) {
	this->sidesProb = sides;
	this->wallsProb = walls;
	this->swapProb = swap;
	this->singleProb = single;
}

void LineBuilder::setStartConfiguration(const LineInfo & line) {
	prevLine = line;
	prevSidesLine = line;
}

void LineBuilder::generateFirstLine(LineInfo & line) {
	generateBalls(line);
}

void LineBuilder::generateBalls(LineInfo & line) {
	int prob = RandomHelper::random_int(0, 100);
	if (prob < sidesProb) {
		prob = RandomHelper::random_int(0, 100);
		if (prob < swapProb) {
			generateSwap(line);
		}
		else {
			line.left = prevSidesLine.left;
			line.right = prevSidesLine.right;
		}
		generateBallCount(line);
	}
	else {
		line.middle = LineInfo::Element::violet;
	}
}

void LineBuilder::generateWalls(LineInfo & line) {
	int prob = RandomHelper::random_int(0, 100);
	LineInfo::Element element = (prob < wallsProb) ? LineInfo::Element::green : LineInfo::Element::none;
	if (line.middle == LineInfo::Element::violet) {
		line.left = line.right = element;
	}
	else {
		line.middle = element;
	}
}

void LineBuilder::generateSwap(LineInfo & line) {
	line.left = getOppositeElement(prevSidesLine.left);
	line.right = getOppositeElement(prevSidesLine.right);
	std::swap(prevSidesLine.left, prevSidesLine.right);
}

LineInfo::Element LineBuilder::getOppositeElement(LineInfo::Element element) {
	if (element == LineInfo::Element::none)
		return LineInfo::Element::none;
	return element == LineInfo::Element::red ? LineInfo::Element::blue : LineInfo::Element::red;
}

void LineBuilder::generateBallCount(LineInfo & line) {
	int prob = RandomHelper::random_int(0, 100);
	if (prob < singleProb) {
		if (RandomHelper::random_int(0, 100) % 2 == 0) {
			line.left = LineInfo::Element::none;
		}
		else {
			line.right = LineInfo::Element::none;
		}
	}
}