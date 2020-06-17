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
#include "GridPathfinder.h"
#include <vector>
#include "PathfindingList.h"

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

class DijkstraPathfinder : public GridPathfinder
{
public:
	DijkstraPathfinder(Graph* pGraph);
	~DijkstraPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);
};

