//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-405-01
//Project 2: Steering Behaviors
//10/26/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

#include "Vector2D.h"

const int LOCAL_BOID_NUM = 25;
const int TOTAL_BOID_NUM = 50;

enum GameMessages
{
	ID_CUSTOM_MESSAGE_START = ID_USER_PACKET_ENUM,

	//Custom message types that can be sent
	ID_CHAT_MESSAGE_1,
	SERVER_MESSAGE,
	SEND_USER_DATA,
	SEND_NETWORK_MODEL,
	DATA_PUSH_MESSAGE,
	DATA_SHARE_MESSAGE,
	DATA_COUPLE_MESSAGE,
	OBSTACLE_EVENT_MESSAGE,
	PLAYER_QUIT_MESSAGE,
	GAME_EVENT
};

enum NetworkModel
{
	INVALID_TYPE = -1,
	DATA_PUSH,
	DATA_SHARING,
	DATA_COUPLING
};

//Structs that can be sent via RakNet
#pragma pack(push, 1)

struct DataObject
{
	Vector2D mPosVec;
	Vector2D mVel;
	Vector2D mAcc;
	float mFacing;
	bool mLocal;
};

struct DataPushMessage
{
	unsigned char typeID = DATA_PUSH_MESSAGE;

	DataObject arr[TOTAL_BOID_NUM];

	DataObject obstacles[TOTAL_BOID_NUM];

	int numOfObstacles = 0;
};

//This can be consdensed with data couple message if needed.
//Right now clarity outways program file size.
struct DataShareMessage
{
	unsigned char typeID = DATA_SHARE_MESSAGE;

	DataObject arr[LOCAL_BOID_NUM];
};

struct DataCoupleMessage
{
	unsigned char typeID = DATA_COUPLE_MESSAGE;

	DataObject arr[LOCAL_BOID_NUM];
};

struct UserData
{
	unsigned char typeID = SEND_USER_DATA;

	RakNet::SystemAddress address;
	RakNet::RakNetGUID userGUID;
	int timeStamp;
};

// Used to notify clients of the networking model in use
struct NetworkModelMessage
{
	unsigned char typeID = SEND_NETWORK_MODEL;

	NetworkModel modelType;
};

struct ServerMessage
{
	unsigned char typeID = SERVER_MESSAGE;
};

struct ObstacleEventMessage
{
	unsigned char typeID = OBSTACLE_EVENT_MESSAGE;

	Vector2D obstaclePos;
};

#pragma pack (pop)

struct NetworkInfo
{
	// generic game info
	RakNet::RakPeerInterface *peer;
	unsigned short serverPort;
	RakNet::RakNetGUID host_id;
	RakNet::SystemAddress myAddress;

	//Personal Variables
	UserData* clientData;
	int currentClientNum = 0;
	const unsigned int MAXCLIENTS = 3;

	NetworkModel model;

	//FLAGS

	//Designates the game is ready to start
	char startGame = 0;
	// Designates the characters can move
	char isRunning = 0;
};

//Proccesses all the networking
void handleNetworking();

void hostGame();

void getModel();

// Removes newline from the end of fgets for cleaner work using C.
// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
char* fgetstr(char*, int, FILE*);