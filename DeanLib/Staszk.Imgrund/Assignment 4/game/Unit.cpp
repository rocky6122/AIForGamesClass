//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "Unit.h"
#include <assert.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "SpriteManager.h"


Unit::Unit(const Sprite& sprite, Vector2D position)
	:mSprite(sprite)
	,mPosition(position)
	, mShowTarget(false)
{
	mNavAgent = new NavAgent(mPosition);
}

Unit::~Unit()
{
}

void Unit::update(float elapsedTime)
{
	mNavAgent->update(elapsedTime);
	mPosition = mNavAgent->getPosition();
}

void Unit::draw() const
{
	gpGame->getGraphicsSystem()->draw(mSprite, mPosition.getX(), mPosition.getY());

	//IF WE WANT TO IMPLEMENT SHOW TARGET AFTER INTEGRATION OF NAVAGENT. PLACE HERE
	if (mShowTarget)
	{
	/*	const Vector2D& targetLoc = LOCATION OF GOAL NODE;
		if (&targetLoc != &ZERO_VECTOR2D)
		{
			Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			assert(pTargetSprite != NULL);
			gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
		}*/
	}
}