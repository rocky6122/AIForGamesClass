//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "PathfinderManager.h"
#include "DijkstraPathfinder.h"
#include "DepthFirstPathfinder.h"
#include "AStarPathfinder.h"
#include "SmoothAStar.h"



PathfinderManager::PathfinderManager(Graph* pGraph)
{
	mpGraph = pGraph;

	openList.emplace_back(new DepthFirstPathfinder(pGraph));
	openList.emplace_back(new DepthFirstPathfinder(pGraph));

	openList.emplace_back(new DijkstraPathfinder(pGraph));
	openList.emplace_back(new DijkstraPathfinder(pGraph));

	openList.emplace_back(new AStarPathfinder(pGraph));
	openList.emplace_back(new AStarPathfinder(pGraph));

	openList.emplace_back(new SmoothAStar(pGraph));
	openList.emplace_back(new SmoothAStar(pGraph));

	openList.emplace_back(new SmoothAStar(pGraph));
	openList.emplace_back(new SmoothAStar(pGraph));
}


PathfinderManager::~PathfinderManager()
{
	GridPathfinder* pathfinder;

	while(openList.size() > 0)
	{
		pathfinder = openList.front();
		openList.pop_front();

		delete pathfinder;
	}

	while (closedList.size() > 0)
	{
		pathfinder = closedList.front();
		closedList .pop_front();

		delete pathfinder;
	}
}

GridPathfinder * PathfinderManager::requestPathfinder()
{
	if (openList.size() < 1)
	{
		return nullptr;
	}

	GridPathfinder* moveToClose = openList.front();

	openList.pop_front();

	closedList.emplace_back(moveToClose);

	switch (moveToClose->getID())
	{
	case 1:
		printf("Depth First Pathfinder selected.\n");
		break;
	case 2:
		printf("Dijkstra Pathfinder selected.\n");
		break;
	case 3:
		printf("A* Pathfinder selected.\n");
		break;
	case 4:
		printf("Smooth A* Pathfinder selected.\n");
		break;
	default:
		printf("ERROR. NO PATHFINDING TYPE SELECTED.\n");
		break;
	}

	return moveToClose;
}

void PathfinderManager::freePathfinder(GridPathfinder * pathToFree)
{
	closedList.remove(pathToFree);

	openList.emplace_back(pathToFree);
}

void PathfinderManager::setPathfinders(PathfindingType type)
{
	GridPathfinder* pathfinder;

	while (openList.size() > 0)
	{
		pathfinder = openList.front();
		openList.pop_front();

		delete pathfinder;
	}

	while (closedList.size() > 0)
	{
		pathfinder = closedList.front();
		closedList.pop_front();

		delete pathfinder;
	}


	for (int i = 0; i < M_POOL_SIZE; ++i)
	{
		switch (type)
		{
		case DFS:
		{
			pathfinder = new DepthFirstPathfinder(mpGraph);
			printf("Setting Pathfinding: Depth First\n");
		}
		break;

		case DIJKSTRA:
		{
			pathfinder = new DijkstraPathfinder(mpGraph);
			printf("Setting Pathfinding: Dijkstra\n");
		}
		break;

		case A_STAR:
		{
			pathfinder = new AStarPathfinder(mpGraph);
			printf("Setting Pathfinding: A*\n");
		}
		break;

		case SMOOTH_A_STAR:
		{
			pathfinder = new SmoothAStar(mpGraph);
			printf("Setting Pathfinding: Smooth A* First\n");
		}
		break;

		default:
			return;
			break;
		}

		openList.emplace_back(pathfinder);
	}
}