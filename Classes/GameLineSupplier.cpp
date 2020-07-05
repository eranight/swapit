#include "GameLineSupplier.hpp"
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

const int LEFT = 0;
const int MID = 1;
const int RIGHT = 2;
const int NONE = 0;
const int RED = 1;
const int BLUE = 2;
const int PURPL = 3;
const int WALL = 4;
const int RED_BLUE = 1;
const int BLUE_RED = 2;

GameLineSupplier::GameLineSupplier(const LineInfo & initialLine, std::vector<LevelProbabilities> levels) :
	mt(random_device()()),
	generator(1, 100),
	levels(levels) {
	reset(initialLine);
}

void GameLineSupplier::reset(const LineInfo & initialLine) {
	this->initialLine = initialLine;
	line = { Action::stand, Amount::one, Wall::no, { 0, 0, 0 }, (initialLine.getLeft() == LineInfo::Element::red ? RED_BLUE : BLUE_RED) };
	currentLevel = levels.begin();
	leftside = rightside = 50;
}

LineInfo GameLineSupplier::getNextLine() {
	int mask[3] = {0, };
	int position;
	Action action = generateAction(mask, position);
	Amount amount = Amount::one;
	if (action != Action::middle) {
		amount = generateAmount(mask);
	}
	Wall wall = generateWall(mask, action);
	line = { action, amount, wall, { mask[0], mask[1], mask[2] }, position };
	return LineInfo(LineInfo::Element(mask[LEFT]), LineInfo::Element(mask[MID]), LineInfo::Element(mask[RIGHT]));
}

int GameLineSupplier::castDice(vector<const int> events) {
	vector<int> values = vector<int>(events.size(), 0);
	partial_sum(events.begin(), events.end(), values.begin(), plus<int>());
	CCASSERT(values[values.size() - 1] == 100, "the sum of events must be 100, occures " + values[values.size() - 1]);
	int result = generator(mt);
	auto iter = find_if(values.begin(), values.end(), [result](const int & val) { return result <= val; });
	CCASSERT(iter != values.end(), "event must be found in values");
	return distance(values.begin(), iter);
}

GameLineSupplier::Action GameLineSupplier::generateAction(int mask[], int & position) {
	Action action;
	if (currentLevel->forceSwap || currentLevel->forceMiddle || line.action != Action::middle) {
		if (currentLevel->forceSwap) {
			currentLevel->forceSwap = false;
			action = Action::swap;
		}
		else if (currentLevel->forceMiddle) {
			currentLevel->forceMiddle = false;
			action = Action::middle;
		}
		else {
			action = Action(castDice(currentLevel->action));
		}
	}
	else { // �������� ���� ������ � ������, ���� ���������� �������� ���� "Middle", ��� ������ ��������� ����� ������ ���� ������
		   // ��� ��� ���������� �������� � ����� ������������ ���������� ������
		action = Action::swap;
	}
	if (action == Action::stand) {
		mask[LEFT] = line.position == RED_BLUE ? RED : BLUE;
		mask[MID] = NONE;
		mask[RIGHT] = line.position == RED_BLUE ? BLUE : RED;
		position = line.position;
	}
	else if (action == Action::swap) {
		mask[LEFT] = line.position == RED_BLUE ? BLUE : RED;
		mask[MID] = NONE;
		mask[RIGHT] = line.position == RED_BLUE ? RED : BLUE;
		position = line.position == RED_BLUE ? BLUE_RED : RED_BLUE;
	}
	else {
		mask[MID] = PURPL;
		position = line.position;
	}
	return action;
}

GameLineSupplier::Amount GameLineSupplier::generateAmount(int mask[]) {
	Amount amount;
	if (currentLevel->forceDouble) {
		currentLevel->forceDouble = false;
		amount = Amount::two;
	}
	else {
		amount = Amount(castDice(currentLevel->amount));
	}
	if (amount == Amount::one) {
		int side = castDice({ leftside, rightside });
		if (side == 0) {      // ���������� � ����� �������
			leftside -= 10;   // ��������� ����������� ��������� ��������� �����
			rightside += 10;  // � ��������������� ����������� ����������� ��������� ��������� ������
			mask[RIGHT] = 0;  // �������� ����� � ������ �����, ��� ������ ���������� ������ ������
		}
		else {                // ���������� � ������ ������� (����������)
			leftside += 10;
			rightside -= 10;
			mask[LEFT] = 0;
		}
	}
	return amount;
}

GameLineSupplier::Wall GameLineSupplier::generateWall(int mask[], GameLineSupplier::Action action) {
	Wall wall;
	if (currentLevel->forceWall) {
		currentLevel->forceWall = false;
		wall = Wall::yes;
	}
	else {
		wall = Wall(castDice({ currentLevel->wall, 100 - currentLevel->wall }));
	}
	if (wall == Wall::yes) {
		if (action == Action::middle) {
			mask[LEFT] = mask[RIGHT] = WALL;
		}
		else {
			mask[MID] = WALL;
		}
	}
	return wall;
}