//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "SmoothAStar.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Game.h"
#include "Grid.h"


SmoothAStar::SmoothAStar(Graph* pGraph) : AStarPathfinder(pGraph)
{
	ID = 4;
}


SmoothAStar::~SmoothAStar()
{
}

Path * SmoothAStar::findPath(Node * pFrom, Node * pTo)
{
	// Finds standard A* path
	mpPath = AStarPathfinder::findPath(pFrom, pTo);

	// Ensures that the path length is at least three nodes
	if (mpPath->getNumNodes() < 3)
	{
		return mpPath;
	}

	// Creates a new graph and sets first node
	Path* smoothPath = new Path();
	smoothPath->addNode(mpPath->peekNode(0));

	// We start at 2, because we assume two adjacent nodes will pass the ray cast
	int inputIndex = 2;

	// Loop until we find the last item in the member path
	while (inputIndex < mpPath->getNumNodes())
	{
		// If the ray cast is not clear,
		// we add the previous node to the path
		Vector2D positionVec(pFrom->getXPos() * 32, pFrom->getYPos() * 32);
		Vector2D directionVec(mpPath->peekNode(inputIndex)->getXPos() * 32, mpPath->peekNode(inputIndex)->getYPos() * 32);

		directionVec -= positionVec;


		if (raycast(positionVec, directionVec, 800.0f))
		{
			//RayCast hit something, add the last node that passed to the list
			smoothPath->addNode(mpPath->peekNode(inputIndex - 1));
		}

		++inputIndex;
	}

	smoothPath->addNode(pTo);

	//Set mpPath = to smoothPath to avoid a memory leak of the smoothPath* we create
	delete mpPath;
	mpPath = smoothPath;

	return mpPath;
}

void SmoothAStar::swap(int& lhs, int& rhs)
{
	int temp = lhs;
	lhs = rhs;
	rhs = temp;
}

std::vector<Vector2D> SmoothAStar::BresenhamLine(const Vector2D & pointOne, const Vector2D & pointTwo)
{
	return BresenhamLine((int)pointOne.getX(), (int)pointOne.getY(), (int)pointTwo.getX(), (int)pointTwo.getY());
}

std::vector<Vector2D> SmoothAStar::BresenhamLine(int pointOneX, int pointOneY, int pointTwoX, int pointTwoY)
{
	std::vector<Vector2D> result;

	bool steep = abs(pointTwoY - pointOneY) > abs(pointTwoX - pointOneX);

	// By switching the x and y axis,
	// we can check positive and negative steep gradients
	if (steep)
	{
		swap(pointOneX, pointOneY);
		swap(pointTwoX, pointTwoY);
	}
	
	// reverses the input coordinates for drawing
	if (pointOneX > pointTwoX)
	{
		swap(pointOneX, pointTwoX);
		swap(pointOneY, pointTwoY);
	}


	int deltaX = pointTwoX - pointOneX;
	int deltaY = abs(pointTwoY - pointOneY);
	int error = 0;
	int yStep;
	int yVar = pointOneY;

	if (pointOneY < pointTwoY)
	{
		yStep = 1;
	}
	else
	{
		yStep = -1;
	}

	for (int xVar = pointOneX; xVar < pointTwoX; xVar++)
	{
		if (steep)
		{
			result.emplace_back(Vector2D(yVar, xVar));
		}
		else
		{
			result.emplace_back(Vector2D(xVar, yVar));
		}

		error += deltaY;

		if (2 * error >= deltaX)
		{
			yVar += yStep;
			error -= deltaX;
		}
	}
	
	return result;
}

bool SmoothAStar::raycast(Vector2D position, Vector2D direction, float rayLength)
{
	if (rayLength == 0)
	{
		return false;
	}

	direction.normalize();

	std::vector<Vector2D> rayLine = BresenhamLine(position, position + (direction * rayLength));

	if (rayLine.size() > 0)
	{
		int rayPointIndex = 0;

		if (rayLine[0] != position)
		{
			rayPointIndex = rayLine.size() - 1;
		}

		while (true)
		{
			Vector2D rayVec = rayLine[rayPointIndex];

			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

			if (pGame->getGrid()->getValueAtPixelXY((int)rayVec.getX(), (int)rayVec.getY()) == BLOCKING_VALUE)
			{
				return true;
			}

			if (rayLine[0] != position)
			{
				rayPointIndex--;

				if (rayPointIndex < 0)
				{
					break;
				}
			}
			else
			{
				rayPointIndex++;

				if (rayPointIndex >= rayLine.size())
				{
					break;
				}
			}
		}
	}

	return false;
}
