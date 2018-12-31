#ifndef __BLOCKING_H__
#define __BLOCKING_H__

class Blocking {
public:
	Blocking(): blocked(false) {}
	virtual ~Blocking() {}
public:
	bool isBlocking() { return blocked; }
	virtual void block() { blocked = true; }
	virtual void unblock() { blocked = false; }
protected:
	bool blocked;
};

#endif //__BLOCKING_H__