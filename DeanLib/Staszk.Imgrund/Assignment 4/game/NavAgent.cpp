//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "NavAgent.h"
#include "NavAgent.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "GridPathfinder.h"
#include "PathfinderManager.h"

NavAgent::NavAgent(Vector2D location)
	:mCurrentIndex(0)
	,mIsPathing(false)
	,mSpeed(90.0f)
	,mCurrentLocation(location)
	,mpPath(NULL)
{
}

NavAgent::~NavAgent()
{
	if (mpPath != NULL)
	{
		delete mpPath;
		mpPath = NULL;
	}
}

void NavAgent::update(float elapsedTime)
{
	if (mIsPathing)
	{
		Vector2D toVector(mpPath->peekNode(mCurrentIndex)->getXPos() * 32.0f, mpPath->peekNode(mCurrentIndex)->getYPos() * 32.0f);

		if (moveTowards(toVector, elapsedTime * mSpeed))
		{
			mCurrentIndex++;

			if (!mpPath->peekNode(mCurrentIndex))
			{
				finishPathfinding();
			}
		}
	}
}

bool NavAgent::moveTowards(const Vector2D & toVector, float delta)
{
	Vector2D vec = toVector - mCurrentLocation;

	float magnitude = vec.getLength();

	if (magnitude <= delta || magnitude == 0.0f)
	{
		mCurrentLocation = toVector;
		return true;
	}

	mCurrentLocation = mCurrentLocation + ( vec / magnitude * delta);
	return false;
}

void NavAgent::setPath(const Vector2D & toVector)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		GridPathfinder* pPathfinder = pGame->getPathfinderManager()->requestPathfinder();

		if (pPathfinder == nullptr)
		{
			return;
		}

		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)mCurrentLocation.getX(), (int)mCurrentLocation.getY());
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)toVector.getX(), (int)toVector.getY());
		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);
		mpPath = pPathfinder->findPath(pFromNode, pToNode);
		pGame->getPathfinderManager()->freePathfinder(pPathfinder);
	}
}

void NavAgent::beginPathing(const Vector2D& toVector)
{
	if (mpPath != NULL)
	{
		delete mpPath;
	}

	setPath(toVector);
	mCurrentIndex = 0;
	mIsPathing = true;
}

void NavAgent::finishPathfinding()
{
	mIsPathing = false;
	mCurrentIndex = 0;
	delete mpPath;
	mpPath = NULL;
}
