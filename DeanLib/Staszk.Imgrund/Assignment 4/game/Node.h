#pragma once

#include <Trackable.h>

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id , const int x, const int y);
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};

	const int getXPos() const { return mXPos; }
	const int getYPos() const { return mYPos; }

private:
	const NODE_ID mId;
	const int mXPos;
	const int mYPos;
};