#ifndef __BLOCKABLE_H__
#define __BLOCKABLE_H__

class Blockable {
public:
	virtual ~Blockable() {}
public:
	virtual void block() = 0;
	virtual void unblock() = 0;
};

#endif //__BLOCKABLE_H__