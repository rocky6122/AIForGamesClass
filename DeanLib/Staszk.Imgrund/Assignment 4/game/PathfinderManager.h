//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once

#include "GridPathfinder.h"
#include "GameApp.h"
#include <list>

class PathfinderManager
{
public:
	PathfinderManager(Graph* pGraph);
	~PathfinderManager();

	GridPathfinder* requestPathfinder();
	void freePathfinder(GridPathfinder* pathToFree);

	void setPathfinders(PathfindingType type);

private:
	const int M_POOL_SIZE = 10;

	std::list<GridPathfinder*> openList;
	std::list<GridPathfinder*> closedList;

	Graph* mpGraph;
};

