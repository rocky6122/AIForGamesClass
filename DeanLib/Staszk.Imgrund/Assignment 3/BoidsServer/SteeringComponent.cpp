#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"
#include "Align.h"
#include "ArriveAndFaceSteering.h"
#include "Separation.h"
#include "Cohesion.h"
#include "VelocityMatching.h"
#include "Flocking.h"
#include "Game.h"

SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
		case Steering::SEEK:
		{
			delete mpSteering;
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::ARRIVE:
		{
			delete mpSteering;
			mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::ALIGN:
		{
			delete mpSteering;
			mpSteering = new Align(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::FACE:
		{
			delete mpSteering;
			mpSteering = new FaceSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::ARRIVEANDFACE:
		{
			delete mpSteering;
			mpSteering = new ArriveAndFaceSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::SEPARATION:
		{
			delete mpSteering;
			mpSteering = new Separation(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::COHESION:
		{
			delete mpSteering;
			mpSteering = new Cohesion(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::VELOCITYMATCHING:
		{
			delete mpSteering;
			mpSteering = new VelocityMatching(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::FLOCKING:
		{
			delete mpSteering;
			mpSteering = new Flocking(data.ownerID, data.targetLoc, data.targetID, gpGame->cohesionWeight, gpGame->separationWeight, gpGame->velocityMatchWeight);
			break;
		}

		default:
		{

		}
	};
}

