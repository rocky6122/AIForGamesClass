#pragma once

#include "Trackable.h"

//Modified enum to hold new message types - John Imgrund 9/9/18
enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	PLAYER_EXIT_MESSAGE = 1,
	UNIT_DELETION_MESSAGE = 2,
	UNIT_ADDITION_MESSAGE = 3,
	FLOCKING_WEIGHT_COHESION = 4,
	FLOCKING_WEIGHT_SEPARATION = 5,
	FLOCKING_WEIGHT_VELOCITY_MATCHING = 6,
	GAME_SAVE_LOAD = 7,
	OBJECT_PLACEMENT_MESSAGE = 8
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

