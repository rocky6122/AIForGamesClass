//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18

// Modified GameApp.cpp to include functions created by us.  Adds elements to the program


#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "DijkstraPathfinder.h"
#include "AStarPathfinder.h"
#include "SmoothAStar.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "InputSystem.h"
#include "UnitManager.h"
#include "PathfinderManager.h"

#include <SDL.h>
#include <fstream>
#include <vector>

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";

const float LOOP_TARGET_TIME = 33.3f;

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
,mpInputSystem(NULL)
,mpUnitManager(NULL)
,mpPathfinderManager(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();

	mpInputSystem = new InputSystem();
	mpUnitManager = new UnitManager(10);

	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	// SETS PATHFINDER DEFAULT
	//mpPathfinder = new AStarPathfinder(mpGridGraph);
	mpPathfinderManager = new PathfinderManager(mpGridGraph);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mUnitBufferID, "Unit30.png");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	GraphicsBuffer* pUnitBuffer = mpGraphicsBufferManager->getBuffer(mUnitBufferID);
	if (pUnitBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(UNIT_SPRITE_ID, pUnitBuffer, 0, 0, (float)pUnitBuffer->getWidth(), (float)pUnitBuffer->getHeight());
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputSystem;
	mpInputSystem = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	/*delete mpPathfinder;
	mpPathfinder = NULL;*/

	delete mpPathfinderManager;
	mpPathfinderManager = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw( *pBackBuffer );
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

	//Display
	mpDebugDisplay->draw( pBackBuffer );

	//Process Game Messages
	mpMessageManager->processMessagesForThisframe();

	//Input Commands
	mpInputSystem->processInput(mpMessageManager, this);

	// Update all units
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpUnitManager->drawAll();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::setPathFinding(PathfindingType type)
{
	delete mpPathfinder;
	delete mpDebugDisplay;

	switch (type)
	{
	case DFS:
	{
		mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
		printf("Setting Pathfinding: Depth First\n");
	}
		break;

	case DIJKSTRA:
	{
		mpPathfinder = new DijkstraPathfinder(mpGridGraph);
		printf("Setting Pathfinding: Dijkstra\n");
	}
		break;

	case A_STAR:
	{
		mpPathfinder = new AStarPathfinder(mpGridGraph);
		printf("Setting Pathfinding: A*\n");
	}
		break;

	case SMOOTH_A_STAR:
	{
		mpPathfinder = new SmoothAStar(mpGridGraph);
		printf("Setting Pathfinding: Smooth A* First\n");
	}
	break;

	default:
		break;
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}
