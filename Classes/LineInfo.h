#ifndef __LINE_INFO_H__
#define __LINE_INFO_H__

#include "cocos2d\cocos\base\CCValue.h"

class LineInfo {
public: //new types
	enum class Element {
		none = 0,
		red,
		blue,
		violet,
		green
	};

private:
	static Element getElementByString(const std::string & elementName) {
		return elementName.compare("red") == 0 ? Element::red :
			(elementName.compare("blue") == 0 ? Element::blue :
			(elementName.compare("violet") == 0 ? Element::violet :
			(elementName.compare("green") == 0 ? Element::green : Element::none)));
	}

public: //constructors
	LineInfo() : left(Element::none), right(Element::none), middle(Element::none) {}
	LineInfo(Element left, Element middle, Element right) : left(left), right(right), middle(middle) {}
	LineInfo(const cocos2d::Value & value) : left(getElementByString(value.asValueVector()[0].asString())),
		middle(getElementByString(value.asValueVector()[1].asString())),
		right(getElementByString(value.asValueVector()[2].asString())) {}

public: //interface
	LineInfo::Element getLeft() { return left; }
	LineInfo::Element getMiddle() { return middle; }
	LineInfo::Element getRight() { return right; }

private: //fields
	LineInfo::Element left;
	LineInfo::Element middle;
	LineInfo::Element right;

	friend class LineBuilder;
};

#endif //__LINE_INFO_H__