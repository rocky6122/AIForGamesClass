#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/


//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18

// Modified GameApp.cpp to include functions created by us.  Adds elements to the program

#include "Game.h"

//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputSystem;
class UnitManager;
class PathfinderManager;

enum PathfindingType
{
	INVALID_TYPE = -1,
	DFS,
	DIJKSTRA,
	A_STAR,
	SMOOTH_A_STAR,
	DYNAMIC,
};

class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	void setPathFinding(PathfindingType type);

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	//inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline PathfinderManager* getPathfinderManager() { return mpPathfinderManager; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline InputSystem* getInputSystem() { return mpInputSystem; };
	inline PathfindingType getPathfindingType() { return mCurrentType; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };

private:
	GameMessageManager* mpMessageManager;
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	DebugDisplay* mpDebugDisplay;

	GridPathfinder* mpPathfinder;
	PathfinderManager* mpPathfinderManager;

	InputSystem* mpInputSystem;
	UnitManager* mpUnitManager;

	PathfindingType mCurrentType = DFS;
};

