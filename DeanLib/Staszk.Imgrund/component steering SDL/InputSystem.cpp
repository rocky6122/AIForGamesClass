//Created by John Imgrund
//Last Modified: 9/7/18

#include <SDL.h>

#include "InputSystem.h"
#include "GameMessageManager.h"
#include "Defines.h"
#include "Game.h"

#include "PlayerMoveToMessage.h"
#include "PlayerExitMessage.h"
#include "UnitDeletionMessage.h"
#include "UnitAdditionMessage.h"


InputSystem::InputSystem()
	:mPressedSpaceLast(false)
{
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

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		Vector2D pos(x, y);
		GameMessage* pMessage = new PlayerMoveToMessage(pos);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

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
	else if (state[SDL_SCANCODE_D]) //If D is pressed than delete a random unit
	{
		GameMessage* pMessage = new UnitDeletionMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);					
	}
	//else if (state[SDL_SCANCODE_RETURN]) //Adds a random unit that wanders and chases the player
	//{
	//	GameMessage* pMessage = new UnitAdditionMessage();
	//	MESSAGE_MANAGER->addMessage(pMessage, 0);
	//}
	else if (state[SDL_SCANCODE_SPACE] && !mPressedSpaceLast)
	{
		mPressedSpaceLast = true;
		GameMessage* pMessage = new AIToggleMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (!state[SDL_SCANCODE_SPACE])
	{
		mPressedSpaceLast = false;
	}
}

bool InputSystem::waitingForInput()
{
	SDL_PumpEvents();

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_RETURN])
	{
		return true;
	}

	return false;
}
