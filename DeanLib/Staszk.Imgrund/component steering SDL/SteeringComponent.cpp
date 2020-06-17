#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"
#include "Align.h"
#include "WanderSteering.h"
#include "WanderandChase.h"
#include "ArriveAndFaceSteering.h"
#include "Separation.h"

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
			//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			//create new steering
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, false);
			break;
		}
		case Steering::FLEE:
		{
			delete mpSteering;
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, true);
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
		case Steering::WANDER:
		{
			delete mpSteering;
			mpSteering = new WanderSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::WANDERANDCHASE:
		{
			delete mpSteering;
			mpSteering = new WanderandChase(data.ownerID, data.targetLoc, data.targetID);
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

		default:
		{
			delete mpSteering;
			mpSteering = NULL;
		}
	};
}

