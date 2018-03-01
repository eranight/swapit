#ifndef __LINE_INFO_H__
#define __LINE_INFO_H__

class LineInfo {
public: //new types
	enum class Element {
		none = 0,
		red,
		blue,
		violet,
		green
	};

public: //constructors
	LineInfo() : left(Element::none), right(Element::none), middle(Element::none) {}
	LineInfo(Element left, Element middle, Element right) : left(left), right(right), middle(middle) {}

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