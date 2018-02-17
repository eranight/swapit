#include "LineBuilder.h"
#include "cocos2d.h"

USING_NS_CC;

LineBuilder::LineBuilder() : lineCounter(0), sidesProb(50), wallsProb(50), swapProb(50), singleProb(50) {}

LineInfo LineBuilder::getNextLine() {
	LineInfo lineInfo;

	if (lineCounter == 0) {
		generateFirstLine(lineInfo);
	}
	else {
		generateBalls(lineInfo);
		generateWalls(lineInfo);
	}

	prevLine = lineInfo;
	++lineCounter;
	return lineInfo;
}

void LineBuilder::generateFirstLine(LineInfo & line) {
	line.left = LineInfo::Element::blue;
	line.middle = LineInfo::Element::none;
	line.right = LineInfo::Element::red;
	prevSidesLine = line;
}

void LineBuilder::generateBalls(LineInfo & line) {
	if (prevLine.middle == LineInfo::Element::violet) {
		generateSwap(line);
	}
	else {
		int prob = RandomHelper::random_int(0, 100);
		if (prob < sidesProb) {
			prob = RandomHelper::random_int(0, 100);
			if (prob < swapProb) {
				generateSwap(line);
				prevSidesLine = line; //line can be incomplete, but it is not important
			}
			else {
				line.left = prevLine.left;
				line.right = prevLine.right;
			}
			prob = RandomHelper::random_int(0, 100);
			if (prob < singleProb) {
				if (RandomHelper::random_int(0, 100) % 2 == 0) {
					line.left = LineInfo::Element::none;
				}
				else {
					line.right = LineInfo::Element::none;
				}
			}
		}
		else {
			line.middle = LineInfo::Element::violet;
		}
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
}

LineInfo::Element LineBuilder::getOppositeElement(LineInfo::Element element) {
	return element == LineInfo::Element::red ? LineInfo::Element::blue : LineInfo::Element::red;
}