#pragma once

#include "Trackable.h"

//Modified enum to hold new message types - John Imgrund 9/9/18
enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_EXIT_MESSAGE = 0,
	//messages shared over server
	//Numbers are fixed based on client message numbers
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

