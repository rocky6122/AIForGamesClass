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
#include "GameMessageManager.h"
#include "ObjectPlacementMessage.h"
#include "Defines.h"

void handleNetworking()
{
	//READ
	RakNet::Packet *packet;

	for (
		packet = gpGame->getNetworkInfo()->peer->Receive();
		packet;
		gpGame->getNetworkInfo()->peer->DeallocatePacket(packet), packet = gpGame->getNetworkInfo()->peer->Receive()
		)
	{
		switch (packet->data[0])
		{
		case ID_TIMESTAMP:
		{
			RakNet::BitStream BS(packet->data, packet->length, false);

			int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;

			gpGame->getNetworkInfo()->peer->Send(&BS, HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
		}
		break;


		case SEND_USER_DATA:
		{
			if (gpGame->getNetworkInfo()->currentClientNum == 0) //Host (Sets host data into the array on first run through)
			{
				UserData hostData;
				hostData.userGUID = gpGame->getNetworkInfo()->peer->GetMyGUID();
				hostData.address = gpGame->getNetworkInfo()->peer->GetSystemAddressFromGuid(gpGame->getNetworkInfo()->peer->GetMyGUID());
				hostData.timeStamp = RakNet::GetTime();
				gpGame->getNetworkInfo()->clientData[gpGame->getNetworkInfo()->currentClientNum] = hostData;

				gpGame->getNetworkInfo()->currentClientNum++;
			}

			UserData *userData = (UserData *)packet->data;

			gpGame->getNetworkInfo()->clientData[gpGame->getNetworkInfo()->currentClientNum] = *userData;

			gpGame->getNetworkInfo()->currentClientNum++;

			NetworkModelMessage data[1];
			data->modelType = gpGame->getNetworkInfo()->model;
			gpGame->getNetworkInfo()->peer->Send((char*)(data), sizeof(NetworkModelMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

			if (gpGame->getNetworkInfo()->currentClientNum >= 3)
			{
				gpGame->getNetworkInfo()->startGame = 1;

				//Send a message to all client to start the simulation
				
				ServerMessage message[1];

				gpGame->getNetworkInfo()->peer->Send((char*)message, sizeof(ServerMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->myAddress, true);

			}

		}
		break;

		case DATA_SHARE_MESSAGE:
		{
			DataShareMessage *message = (DataShareMessage *)packet->data;

			int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;
			
			gpGame->getNetworkInfo()->peer->Send((char*)(message), sizeof(DataShareMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
		}
		break;

		case DATA_COUPLE_MESSAGE:
		{
			DataCoupleMessage *message = (DataCoupleMessage *)packet->data;
			
			int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;

			gpGame->getNetworkInfo()->peer->Send((char*)(message), sizeof(DataCoupleMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
		}
		break;

		case OBSTACLE_EVENT_MESSAGE:
		{
			switch (gpGame->getNetworkInfo()->model)
			{
			case DATA_PUSH:
			{
				ObstacleEventMessage *message = (ObstacleEventMessage *)packet->data;

				GameMessage* pMessage = new ObjectPlacementMessage(message->obstaclePos);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			break;

			case DATA_SHARING:
			{
				ObstacleEventMessage *message = (ObstacleEventMessage *)packet->data;

				int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;

				gpGame->getNetworkInfo()->peer->Send((char*)(message), sizeof(ObstacleEventMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
			}
			break;

			case DATA_COUPLING:
			{
				ObstacleEventMessage *message = (ObstacleEventMessage *)packet->data;

				int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;

				gpGame->getNetworkInfo()->peer->Send((char*)(message), sizeof(ObstacleEventMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
			}
			break;
			}
			
		}
		break;

		case PLAYER_QUIT_MESSAGE:
		{
			switch (gpGame->getNetworkInfo()->model)
			{
			case DATA_PUSH:
			{
				UnitManager* manager = gpGame->getUnitManager();

				while (manager->getMapSize() > LOCAL_BOID_NUM + 1)
				{
					manager->deleteRecentUnit();
				}
			}
			break;

			case DATA_SHARING:
			{
			}

			case DATA_COUPLING:
			{
				ServerMessage *message = (ServerMessage *)packet->data;

				int otherClient = (gpGame->getNetworkInfo()->clientData[1].userGUID == packet->guid) ? 2 : 1;

				gpGame->getNetworkInfo()->peer->Send((char*)(message), sizeof(ServerMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->clientData[otherClient].userGUID, false);
			}
			break;
			}
		}
		break;

		case ID_DISCONNECTION_NOTIFICATION:
		{
			printf("A client has disconnected.\n");
		}
			break;
		
		case ID_CONNECTION_LOST:	
		{
			printf("A client lost the connection.\n");
		}
			break;
		}
	}

	//WRITES NEEDS TO HAPPEN BASED NETWORK MODELS
	if (gpGame->getNetworkInfo()->isRunning)
	{
		switch (gpGame->getNetworkInfo()->model)
		{
			case DATA_PUSH:
			{
				DataPushMessage msg[1];

				for (int i = 1; i < gpGame->getUnitManager()->getMapSize(); i++)
				{
					Vector2D vec = gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition();
					float face = gpGame->getUnitManager()->getUnit(i)->getFacing();
					bool local = gpGame->getUnitManager()->getUnit(i)->getLocal();

					msg->arr[i - 1].mPosVec = vec;
					msg->arr[i - 1].mFacing = face;
					msg->arr[i - 1].mLocal = local;
				}

				for (int i = 1; i < TOTAL_BOID_NUM + 1; i++)
				{
					if (gpGame->getObstacleManager()->getUnit(i) != NULL)
					{
						Vector2D vec = gpGame->getObstacleManager()->getUnit(i)->getPositionComponent()->getPosition();
						float face = gpGame->getObstacleManager()->getUnit(i)->getFacing();
						bool local = gpGame->getObstacleManager()->getUnit(i)->getLocal();

						msg->obstacles[i - 1].mPosVec = vec;
						msg->obstacles[i - 1].mFacing = face;
						msg->obstacles[i - 1].mLocal = local;

						++msg->numOfObstacles;
					}
					else
					{
						break;
					}
				}

				gpGame->getNetworkInfo()->peer->Send((char*)(msg), sizeof(DataPushMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->myAddress, true);
			}

			break;
		}
	}
}

void hostGame()
{
	printf("YOU ARE A SERVER:\n");

	gpGame->getNetworkInfo()->serverPort = atoi("25656");
	RakNet::SocketDescriptor sd(gpGame->getNetworkInfo()->serverPort, 0);
	gpGame->getNetworkInfo()->peer->Startup(gpGame->getNetworkInfo()->MAXCLIENTS, &sd, 1);

	gpGame->getNetworkInfo()->clientData = new UserData[gpGame->getNetworkInfo()->MAXCLIENTS];


	getModel();

	// We need to let the server accept incoming connections from the clients

	system("cls");
	printf("Starting the Game.\n");
	gpGame->getNetworkInfo()->peer->SetMaximumIncomingConnections(gpGame->getNetworkInfo()->MAXCLIENTS);

	printf("Waiting for clients...");
}

void getModel()
{
	gpGame->getNetworkInfo()->model = DATA_SHARING;
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