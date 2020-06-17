//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>


DijkstraPathfinder::DijkstraPathfinder(Graph* pGraph) : GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif // VISUALIZE_PATH

	ID = 2;
}


DijkstraPathfinder::~DijkstraPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif // VISUALIZE_PATH

}

Path* DijkstraPathfinder::findPath(Node * pFrom, Node * pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	// Initialize record for start node
	NodeRecord* startRecord = new NodeRecord();
	startRecord->mNode = pFrom;
	startRecord->mConnection = NULL;
	startRecord->mCostSoFar = 0;

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif


	// Initialize open and closed lists
	PathfindingList openList = PathfindingList();
	openList.add(startRecord);
	PathfindingList closedList = PathfindingList();

	NodeRecord* current;

	// Iterate through open list, processing each node
	while (openList.getSize() > 0)
	{
		// Find the smallest element in the open list
		current = openList.findSmallest();

		// If it is the end node, then terminate
		if (current->mNode == pTo)
		{
			break;
		}
		// Otherwise get its outgoing connections
		else
		{
			std::vector<Connection*> connections = mpGraph->getConnections(current->mNode->getId());

			// Loop through all connections
			for (auto conn : connections)
			{
				// Get cost estimate for the end node
				NodeRecord* endNodeRecord;
				Node* endNode = conn->getToNode();
				float endNodeCost = current->mCostSoFar + conn->getCost();

				// Skip if the node is closed
				if (closedList.contains(endNode))
				{
					// continue on  ????????
					continue;
				}
				// Or if it is open and we've found a worse route
				else if (openList.contains(endNode))
				{
					// Here we find the record in the open list
					// corresponding to the endNode
					endNodeRecord = openList.find(endNode);
					if (endNodeRecord->mCostSoFar <= endNodeCost)
					{
						// continue on  ?????????
						continue;
					}
				}
				// Otherwise we know we've got an unvisitied node
				// so we make a record of it
				else
				{
					endNodeRecord = new NodeRecord();
					endNodeRecord->mNode = endNode;
				}


				// We need to update the node
				// Update cost and connection
				endNodeRecord->mCostSoFar = endNodeCost;
				endNodeRecord->mConnection = conn;

				// We add it to the open list
				if (!openList.contains(endNode))
				{
					openList.add(endNodeRecord);
				}
			}

			// We've finished looking through the connections
			// for the current node, so add it to the closed
			// list and remove from the open list
			openList.remove(current);
			closedList.add(current);
		}
	}

	// We are either at the goal or there are no
	// more nodes to search
	if (current->mNode != pTo)
	{
		// We did not find a path, so return nothing
		return nullptr;
	}
	else
	{
		std::vector<Node*> tempPath;
		Path* pPath = new Path();

		while (current->mNode != pFrom)
		{
			tempPath.push_back(current->mNode);

			// FIND OUT HOW TO TRAVERSE THE PATH
			current = closedList.find(current->mConnection->getFromNode());
		}

		tempPath.push_back(pFrom);


		for (int i = tempPath.size() - 1; i >= 0; i--)
		{
			pPath->addNode(tempPath[i]);

#ifdef VISUALIZE_PATH
			mVisitedNodes.push_back(tempPath[i]);
#endif
		}

		gpPerformanceTracker->stopTracking("path");
		mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
		return pPath;

	}
}
