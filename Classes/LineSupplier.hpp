#pragma once

class LineSprites;

class LineSupplier {
public:
	virtual LineSprites * getNextLine() = 0;
};