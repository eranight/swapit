#ifndef __LINE_BUILDER_H__
#define __LINE_BUILDER_H__

#include "LineInfo.h"

class LineBuilder {
public: //constuctors
	LineBuilder();

public: //interface
	LineInfo getNextLine();
	void setProbabilities(int sides, int walls, int swap, int sigle);
	void setStartConfiguration(const LineInfo & lineInfo);

private: //methods
	void generateFirstLine(LineInfo &);
	void generateBalls(LineInfo &);
	void generateWalls(LineInfo &);

	void generateSwap(LineInfo &);
	void generateBallCount(LineInfo &);
	inline LineInfo::Element getOppositeElement(LineInfo::Element);

private: //fields
	int lineCounter;
	LineInfo prevLine;
	LineInfo prevSidesLine; //holds last combination with sides balls

	//probabilities
	int sidesProb;  //on sides or middle
	int wallsProb;  //has walls
	int swapProb;   //forces to swap
	int singleProb; //one or two balls
};

#endif //__LINE_BUILDER_H__