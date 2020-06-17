//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

//Library includes
#include <SDL.h>

//Generic includes for the system to handle Game Messages
#include "InputSystem.h"
#include "GameMessageManager.h"
#include <Defines.h>
#include "GameApp.h"
#include <Vector2D.h>

//Message Types
#include "MessagesList.h"


InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::processInput(GameMessageManager* pMessageManager, GameApp* pGameApp)
{
	//get input - should be moved someplace better
	SDL_PumpEvents();
	int x, y;

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new PathToMessage(lastPos, pos);
			pMessageManager->addMessage(pMessage, 0);
			lastPos = pos;
		}
	}

	//get input - should be moved someplace better
	//all this should be moved to InputManager!!!
	{
		//get keyboard state
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		//if escape key was down then exit the loop
		if (state[SDL_SCANCODE_ESCAPE])
		{
			//convert this into a message to be handled by the gameApp
			GameMessage* pMessage = new ExitMessage();
			pMessageManager->addMessage(pMessage, 0);
		}
		else if (state[SDL_SCANCODE_F]) //Turns pathFinding to Smooth A*
		{
			GameMessage* pMessage = new ChangePathfindingMessage(SMOOTH_A_STAR, pGameApp);
			pMessageManager->addMessage(pMessage, 0);
		}
		else if (state[SDL_SCANCODE_D]) //Turns pathFinding to Dijkstra
		{
			GameMessage* pMessage = new ChangePathfindingMessage(DIJKSTRA, pGameApp);
			pMessageManager->addMessage(pMessage, 0);
		}
		else if (state[SDL_SCANCODE_A]) //Turns pathFinding to A*
		{
			GameMessage* pMessage = new ChangePathfindingMessage(A_STAR, pGameApp);
			pMessageManager->addMessage(pMessage, 0);
		}
		else if (state[SDL_SCANCODE_S]) // Deletes All Units. Spawns 10 units randomly.
		{
			GameMessage* pMessage = new UnitSpawnMessage(pGameApp);
			pMessageManager->addMessage(pMessage, 0);
		}
	}
}
