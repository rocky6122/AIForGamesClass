#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>

#include "Networking.h"

class KinematicUnit;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class InputSystem;

const float LOOP_TARGET_TIME = 100.0f;//how long should each frame of execution take? 10fps = 100ms/frame

class Game:public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();

	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline UnitManager* getObstacleManager() { return mpObstacleManager; };

	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline NetworkInfo* getNetworkInfo() const { return mpNetworkInfo; };
	inline int getWidth() const { return M_WIDTH; };
	inline int getHeight() const { return M_HEIGHT; };
	
	//Had to make this public so the input system could interact with it
	//If a better way is found to do this please execute it and let me know - John Imgrund
	bool mShouldExit;

	float cohesionWeight = 2.0f;
	float separationWeight = 1.0f;
	float velocityMatchWeight = 1.0f;

	// Updates the weights for the flocking boids
	void updateFlock();

	// Spawns the boids
	void SpawnBoids();

private:
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	UnitManager* mpObstacleManager;

	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	InputSystem* mpInputSystem; //Added to place an input system into the game - John Imgrund
	NetworkInfo* mpNetworkInfo;
	
	const int M_WIDTH = 512;
	const int M_HEIGHT = 768;

	// functions
	void setUpNetwork();
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;