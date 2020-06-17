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

class AStarPathfinder : public GridPathfinder
{
public:
	AStarPathfinder(Graph* pGraph);
	~AStarPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);

	
	float heuristic(Node* pNode);

private:
	Node* mpGoal;

	// Member variables used in heuristic
	// Distance scale assumes connections have a weight of 1
	const int DISTANCE_SCALE = 1;

	// Diagonal distance
	const float DISTANCE_SCALE_DIAGONAL = 1.414f;
};

