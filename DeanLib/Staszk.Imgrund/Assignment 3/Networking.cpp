//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-405-01
//Project 2: Steering Behaviors
//10/26/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "networking.h"
#include "Game.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "GameMessageManager.h"
#include "ObjectPlacementMessage.h"
#include "Defines.h"

void handleNetworking()
{
	//READ
	RakNet::Packet *packet;

	float msDelay = INFINITY;
	DataShareMessage finalMessage;
	int count = 0;

	for (
		packet = gpGame->getNetworkInfo()->peer->Receive();
		packet;
		gpGame->getNetworkInfo()->peer->DeallocatePacket(packet), packet = gpGame->getNetworkInfo()->peer->Receive()
		)
	{

		switch (packet->data[0])
		{
		//CLIENT READS

		case ID_TIMESTAMP:
		{
			gpGame->getNetworkInfo()->packetRecieved = 1;

			if (gpGame->getNetworkInfo()->hasReceivedBoids == 0)
			{
				gpGame->getNetworkInfo()->hasReceivedBoids = 1;
			}

			RakNet::BitStream BS(packet->data, packet->length, false);

			BS.IgnoreBytes(sizeof(RakNet::MessageID));

			RakNet::Time sentTime;

			BS.Read(sentTime);

			//CHECK TIMESTAMP
			unsigned int time = (unsigned int)RakNet::GetTime() - (unsigned int)sentTime;

			if (time < msDelay)
			{
				msDelay = time;
				BS.Read(finalMessage);
			}
		}
		break;
		
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			//Gathers user data to send to the server as reference.
			UserData data[1];
			data->userGUID = gpGame->getNetworkInfo()->peer->GetMyGUID();
			data->address = gpGame->getNetworkInfo()->peer->GetSystemAddressFromGuid(gpGame->getNetworkInfo()->peer->GetMyGUID());
			data->timeStamp = RakNet::GetTime();
			gpGame->getNetworkInfo()->peer->Send((char*)(data), sizeof(UserData), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
		break;

		case SERVER_MESSAGE:
		{
			gpGame->getNetworkInfo()->startGame = 1;
		}
		break;

		case SEND_NETWORK_MODEL:
		{
			NetworkModelMessage *message = (NetworkModelMessage *)packet->data;
			

			gpGame->getNetworkInfo()->host_id = packet->guid;
			gpGame->getNetworkInfo()->model = message->modelType;
		}

		break;

		case DATA_SHARE_MESSAGE:
		{
			if (gpGame->getNetworkInfo()->hasReceivedBoids == 0)
			{
				gpGame->getNetworkInfo()->hasReceivedBoids = 1;
			}

			DataShareMessage *message = (DataShareMessage *)packet->data;
			UnitManager* manager = gpGame->getUnitManager();

			//printf("Time diff in MS: %d\n", time);

			if (!gpGame->getNetworkInfo()->deadReckonThisFrame)
			{
				while (manager->getMapSize() > LOCAL_BOID_NUM + 1)
				{
					manager->deleteRecentUnit();
				}

				Unit* pUnit;

				for (int i = 0; i < LOCAL_BOID_NUM; i++)
				{
					DataObject obj = message->arr[i];

					pUnit = manager->createUnit(*gpGame->getSpriteManager()->getSprite(1), true, PositionData(obj.mPosVec, obj.mFacing), PhysicsData(obj.mVel, obj.mAcc, 0.0f, 0.0f), INVALID_UNIT_ID, false);
					pUnit->setSteering(Steering::INVALID_TYPE);
				}
			}

			//startReckoning();
			//gpGame->getNetworkInfo()->deadReckonThisFrame = 1;
		}
		break;

		case PLAYER_QUIT_MESSAGE:
		{
			switch (gpGame->getNetworkInfo()->model)
			{
			case DATA_SHARING:
			{
				UnitManager* manager = gpGame->getUnitManager();

				while (manager->getMapSize() > LOCAL_BOID_NUM + 1)
				{
					manager->deleteRecentUnit();
				}
			}
			break;
			}
		}
		break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			printf("We have been disconnected.\n");
			break;
		
		case ID_CONNECTION_LOST:
			{
				printf("Connection lost.\n");
			}
			break;
		}
	}



	//Checks to see if dead reckoning needs to begin
	if (gpGame->getNetworkInfo()->hasReceivedBoids)
	{
		if (gpGame->getUnitManager()->getMapSize() > 26 && gpGame->getNetworkInfo()->packetRecieved == 0) //THIS IF IS TRUE EVERY FRAME WE DONT RECIEVE A PACKET
		{
			startReckoning();
			gpGame->getNetworkInfo()->deadReckonThisFrame = 1;
			gpGame->getNetworkInfo()->hasInterpolated = 0;
		}
		else if (gpGame->getReckoningManager()->getMapSize() > 20 && gpGame->getNetworkInfo()->packetRecieved == 1)
		{
			UnitManager* tempManager = new UnitManager(LOCAL_BOID_NUM);

			Unit* pUnit;

			for (int i = 0; i < LOCAL_BOID_NUM; i++)
			{
				DataObject obj = finalMessage.arr[i];

				pUnit = tempManager->createUnit(*gpGame->getSpriteManager()->getSprite(1), true, PositionData(obj.mPosVec, obj.mFacing), PhysicsData(obj.mVel, obj.mAcc, 0.0f, 0.0f), INVALID_UNIT_ID, false);
				pUnit->setSteering(Steering::INVALID_TYPE);
			}

			//reckons the boids to prep for interpolation
			catchUpReckoning(tempManager, (33.33f/1000.0f), (msDelay/1000.0f));
			catchUpReckoning(gpGame->getReckoningManager(), (33.33f / 1000.0f), 0);

			//The secret sauce
			interpolation(tempManager);
			

			while (tempManager->getMapSize() > 0)
			{
				tempManager->deleteRecentUnit();
			}

			delete tempManager;
			tempManager = NULL;
		}
	}

	//WRITES NEEDS TO HAPPEN BASED NETWORK MODELS
	if (gpGame->getNetworkInfo()->isRunning)
	{
		switch (gpGame->getNetworkInfo()->model)
		{
		case DATA_SHARING:
		{
			if (!gpGame->getNetworkInfo()->hasReceivedBoids)
			{
				DataShareMessage msg[1];

				for (int i = 1; i < LOCAL_BOID_NUM + 1; i++)
				{
					Vector2D vec = gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition();
					float face = gpGame->getUnitManager()->getUnit(i)->getFacing();
					bool local = gpGame->getUnitManager()->getUnit(i)->getLocal();

					msg->arr[i - 1].mPosVec = vec;
					msg->arr[i - 1].mVel = gpGame->getUnitManager()->getUnit(i)->getPhysicsComponent()->getVelocity();
					msg->arr[i - 1].mAcc = gpGame->getUnitManager()->getUnit(i)->getPhysicsComponent()->getAcceleration();
					msg->arr[i - 1].mFacing = face;
					msg->arr[i - 1].mLocal = local;
				}

				gpGame->getNetworkInfo()->peer->Send((char*)msg, sizeof(DataShareMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->host_id, false);
			}
			else
			{
				RakNet::BitStream BS;

				BS.Write((RakNet::MessageID)ID_TIMESTAMP);
				BS.Write(RakNet::GetTime());

				DataShareMessage msg[1];

				for (int i = 1; i < LOCAL_BOID_NUM + 1; i++)
				{
					Vector2D vec = gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition();
					float face = gpGame->getUnitManager()->getUnit(i)->getFacing();
					bool local = gpGame->getUnitManager()->getUnit(i)->getLocal();

					msg->arr[i - 1].mPosVec = vec;
					msg->arr[i - 1].mVel = gpGame->getUnitManager()->getUnit(i)->getPhysicsComponent()->getVelocity();
					msg->arr[i - 1].mAcc = gpGame->getUnitManager()->getUnit(i)->getPhysicsComponent()->getAcceleration();
					msg->arr[i - 1].mFacing = face;
					msg->arr[i - 1].mLocal = local;
				}

				BS.Write(msg);

				gpGame->getNetworkInfo()->peer->Send(&BS, HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->host_id, false);
			}
		}

		break;
		}
	}
}

void catchUpReckoning(UnitManager* tempManager, float elapsedTime, float latency)
{
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	Unit* pUnit;

	float totalLag = elapsedTime + latency;

	for (int i = 1; i < tempManager->getMapSize(); i++)
	{
		pUnit = tempManager->getUnit(i);

		if (!pUnit)
		{
			continue;
		}

		position = pUnit->getPositionComponent()->getPosition();
		velocity = pUnit->getPhysicsComponent()->getVelocity();
		acceleration = pUnit->getPhysicsComponent()->getAcceleration();

		Vector2D newPos = position + (velocity * totalLag) + ((acceleration * 0.5f) * (totalLag * totalLag));

		pUnit->getPositionComponent()->setPosition(newPos);
	}
}

void interpolation(UnitManager * tempManager)
{
	Vector2D distance;

	for (int i = 1; i < LOCAL_BOID_NUM + 1; i++)
	{
		Unit* tempUnit = tempManager->getUnit(i);
		Unit* finalUnit = gpGame->getReckoningManager()->getUnit(i);


		distance = tempUnit->getPositionComponent()->getPosition() - finalUnit->getPositionComponent()->getPosition();

		if (distance.getLength() > THRESHOLD)
		{
			finalUnit->getPositionComponent()->setPosition(tempUnit->getPositionComponent()->getPosition());
			finalUnit->getPositionComponent()->setFacing(tempUnit->getPositionComponent()->getFacing());
			finalUnit->getPhysicsComponent()->setData(tempUnit->getPhysicsComponent()->getData());
		}
		else
		{
			finalUnit->getPositionComponent()->setPosition((finalUnit->getPositionComponent()->getPosition() + tempUnit->getPositionComponent()->getPosition()) / 2);
			finalUnit->getPositionComponent()->setFacing((finalUnit->getFacing() + tempUnit->getFacing()) / 2);
			
			Vector2D newVelocity;
			Vector2D newAcceleration;

			newVelocity = (finalUnit->getPhysicsComponent()->getVelocity() + tempUnit->getPhysicsComponent()->getVelocity()) / 2;
			newAcceleration = (finalUnit->getPhysicsComponent()->getAcceleration() + tempUnit->getPhysicsComponent()->getAcceleration()) / 2;

			finalUnit->getPhysicsComponent()->setData(PhysicsData(newVelocity, newAcceleration, 0.0f, 0.0f));
		}
	}

	gpGame->getNetworkInfo()->hasInterpolated = 1;
}

void startReckoning()
{
	UnitManager* tempUnitManager = gpGame->getUnitManager();

	int tempIndex = 50;
	Unit* pUnit;

	while (tempUnitManager->getMapSize() > 26)
	{
		// Get unit from old manager
		pUnit = tempUnitManager->getUnit(tempIndex--);
		
		// Copy it to new manager
		gpGame->getReckoningManager()->createUnit(pUnit->getSprite(), true, pUnit->getPositionComponent()->getData(), pUnit->getPhysicsComponent()->getData(), INVALID_UNIT_ID, false);
		
		// Delete from old manager
		tempUnitManager->deleteRecentUnit();
	}
}

void joinGame()
{
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];

	//Finds the chatroom to join
	RakNet::SocketDescriptor sd;
	gpGame->getNetworkInfo()->peer->Startup(1, &sd, 1);
	gpGame->getNetworkInfo()->peer->Startup(1, &sd, 1);
	
	printf("YOU ARE A CLIENT:\n");
	printf("Enter server IP or hit enter for 127.0.0.1\n");
	fgets(str, bufferSz, stdin);

	if (str[0] == '\n')
	{
		strcpy(str, "127.0.0.1");
	}

	gpGame->getNetworkInfo()->serverPort = atoi("25656");

	printf("Joining the game.\n");
	gpGame->getNetworkInfo()->peer->Connect(str, gpGame->getNetworkInfo()->serverPort, 0, 0);
}

// Modified by Parker Staszkiewicz and John Imgrund
// Recieved from http://www.siafoo.net/snippet/75
// Removes new line character at the end of line if it exists
char* fgetstr(char *string, int n, FILE *stream)
{
	char *result;

	result = fgets(string, n, stream);

	if (!result)
	{
		return(result);
	}

	if (string[strlen(string) - 1] == '\n')
	{
		string[strlen(string) - 1] = 0;
	}

	return(string);
}