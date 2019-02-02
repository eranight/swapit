#pragma once

#include "LineInfo.h"

class LineSupplier {
public:
	virtual ~LineSupplier() {}
	virtual LineInfo getNextLine() = 0;
};