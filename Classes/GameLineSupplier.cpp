#include "GameLineSupplier.hpp"
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

GameLineSupplier::GameLineSupplier() : mt(random_device()()), generator(1, 100), isFistLine(true), line({Action::middle, Amount::one, Wall::no}) {
}

LineInfo GameLineSupplier::getNextLine() {
	if (isFistLine) {

	}
	return LineInfo();
}

int GameLineSupplier::castDice(initializer_list<int> events) {
	vector<int> values;
	partial_sum(events.begin(), events.end(), values.begin(), plus<int>());
	CCASSERT(values[values.size() - 1] == 100, "the sum of events must be 100, occures " + values[values.size() - 1]);
	int result = generator(mt);
	auto iter = find_if(values.begin(), values.end(), [result](const int & val) { return result <= val; });
	CCASSERT(iter != values.end(), "event must be found in values");
	return *iter;
}