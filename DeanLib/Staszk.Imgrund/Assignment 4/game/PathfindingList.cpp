//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "PathfindingList.h"

PathfindingList::PathfindingList()
{
	mHead = NULL;
	mTail = NULL;
	mSize = 0;
}


PathfindingList::~PathfindingList()
{
}

void PathfindingList::add(NodeRecord * nr)
{
	Element* temp = new Element();
	temp->mData = nr;
	temp->mNext = NULL;

	if (mHead == NULL)
	{
		mHead = temp;
		mTail = temp;
		temp = NULL;
	}
	else
	{
		mTail->mNext = temp;
		mTail = temp;
	}

	mSize++;
}

bool PathfindingList::remove(NodeRecord * nr)
{
	Element* temp = mHead;
	bool found = false;

	while (temp != NULL && !found)
	{
		if (temp->mData == nr)
		{
			found = true;
		}
		else
		{
			temp = temp->mNext;
		}
	}

	// Remove from list
	if (found && temp == mHead)
	{
		mHead = mHead->mNext;
		delete temp;
	}
	else if (found && temp == mTail)
	{
		Element* current = mHead;
		Element* previous;

		while (current->mNext != NULL)
		{
			previous = current;
			current = current->mNext;
		}

		mTail = previous;
		mTail->mNext = NULL;
		delete current;
	}
	else if (found)
	{
		Element* current = mHead;
		Element* previous;

		while (current != temp)
		{
			previous = current;
			current = current->mNext;
		}

		previous->mNext = current->mNext;
		delete current;
	}

	if (found)
	{
		mSize--;
	}

	return found;
}

// Returns NodeRecord with the smallest costSoFar
NodeRecord* PathfindingList::findSmallest()
{
	if (mHead == NULL)
	{
		return nullptr;
	}

	float smallestValue = mHead->mData->mCostSoFar;
	Element* smallestElement = mHead;
	Element* temp = mHead->mNext;

	while (temp != NULL)
	{
		if (temp->mData->mCostSoFar < smallestValue)
		{
			smallestValue = temp->mData->mCostSoFar;
			smallestElement = temp;
		}

		temp = temp->mNext;
	}

	return smallestElement->mData;
}

// Returns the node record with the smallest EstimateCost
NodeRecord * PathfindingList::findSmallestEstimate()
{
	if (mHead == NULL)
	{
		return nullptr;
	}

	float smallestValue = mHead->mData->mCostSoFar;
	Element* smallestElement = mHead;
	Element* temp = mHead->mNext;

	while (temp != NULL)
	{
		if (temp->mData->mCostEstimate < smallestValue)
		{
			smallestValue = temp->mData->mCostEstimate;
			smallestElement = temp;
		}

		temp = temp->mNext;
	}

	return smallestElement->mData;
}

bool PathfindingList::contains(Node* n)
{
	bool contains = false;

	Element* temp = mHead;

	while (!contains && temp != NULL) 
	{
		if (temp->mData->mNode == n)
		{
			contains = true;
		}

		temp = temp->mNext;
	}

	return contains;
}

NodeRecord* PathfindingList::find(Node * n)
{
	Element* temp = mHead;

	while (temp != NULL)
	{
		if (temp->mData->mNode == n)
		{
			return temp->mData;
		}

		temp = temp->mNext;
	}

	return nullptr;
}
