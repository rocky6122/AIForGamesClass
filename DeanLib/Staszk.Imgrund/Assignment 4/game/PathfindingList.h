//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once
#include "Node.h"
#include "Connection.h"

struct NodeRecord
{
	Node* mNode;
	Connection* mConnection;
	float mCostSoFar;
	float mCostEstimate;
};

struct Element
{
	NodeRecord* mData;
	Element* mNext;
};

class PathfindingList
{
public:
	PathfindingList();
	~PathfindingList();

	void add(NodeRecord* nr);
	bool remove(NodeRecord* nr);
	NodeRecord* findSmallest();
	NodeRecord* findSmallestEstimate();
	bool contains(Node* n);
	NodeRecord* find(Node* n);

	int getSize() { return mSize; }

private:
	Element *mHead, *mTail;
	int mSize;
};

