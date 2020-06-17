//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-405-01
//Project 2: Steering Behaviors
//10/26/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include <SDL.h>

#include "InputSystem.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"

#include "PlayerMoveToMessage.h"
#include "PlayerExitMessage.h"
#include "UnitDeletionMessage.h"
#include "UnitAdditionMessage.h"
#include "FlockingCohesionWeightMessage.h"
#include "FlockingSeparationWeightMessage.h"
#include "FlockingVelocityMatchingWeightMessage.h"
#include "SaveLoadMessage.h"
#include "ObjectPlacementMessage.h"


InputSystem::InputSystem()
{
	mMousePressedLastFrame = false;
}

InputSystem::~InputSystem()
{
}


//Processes all inputs on a given frame and attempts to execute all inputs
void InputSystem::processInputs()
{
	SDL_PumpEvents();
	//Mouse Inputs

	int x, y;

	//Keyboard Inputs
	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		//mShouldExit = true;

		GameMessage* pMessage = new PlayerExitMessage(true);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
}