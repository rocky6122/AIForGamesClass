#pragma once

#include "Trackable.h"
#include "Game.h"

//Modified enum to hold new message types - John Imgrund 9/9/18
enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	PLAYER_EXIT_MESSAGE = 1,
	UNIT_DELETION_MESSAGE = 2,
	UNIT_ADDITION_MESSAGE = 3,
	AI_TOGGLE_MESSAGE = 4,
};

class GameMessage: public Trackable
{
public:
	friend class GameMessageManager;

	GameMessage( MessageType type  );
	~GameMessage();

	double getSubmittedTime() const { return mSubmittedTime; };
	double getScheduledTime() const { return mScheduledTime; };

protected:
	MessageType mType;
	double mSubmittedTime;
	double mScheduledTime;

private:
	virtual void process() = 0; 
};

//So small not worth creating a new file over
class AIToggleMessage :public GameMessage
{
public:
	AIToggleMessage() : GameMessage(AI_TOGGLE_MESSAGE) {};
	~AIToggleMessage() {};

	void process() { gpGame->mIsAIControlled = !gpGame->mIsAIControlled; };
};

