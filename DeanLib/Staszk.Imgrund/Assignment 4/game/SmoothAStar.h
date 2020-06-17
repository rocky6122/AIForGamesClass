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
#include "AStarPathfinder.h"
#include "Path.h"
#include <vector>
#include "Vector2D.h"

class SmoothAStar : public AStarPathfinder
{
public:
	SmoothAStar(Graph* pGraph);
	~SmoothAStar();

	Path* findPath(Node* pFrom, Node* pTo);

private:
	
	void swap(int& lhs, int& rhs);
	std::vector<Vector2D> BresenhamLine(const Vector2D& pointOne, const Vector2D& pointTwo);
	std::vector<Vector2D> BresenhamLine(int pointOneX, int pointOneY, int pointTwoX, int pointTwoY);

	bool raycast(Vector2D position, Vector2D direction, float length);


	
	
	Path* mpPath;
};

