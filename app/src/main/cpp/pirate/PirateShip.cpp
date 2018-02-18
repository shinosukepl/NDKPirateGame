#include "PirateShip.h"

PirateShip::PirateShip(android_app* pApplication, TimeManager& pTimeManager,
                       GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBody(),
        mLeftToRight(false),
        mThisWidth(0.0f),
        mThisHeight(0.0f),
        mShipSpeed(40.0f)
{

}

void PirateShip::registerShip(Location& pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void PirateShip::initialize(bool leftToRight)
{
    mLeftToRight = leftToRight;
    spawn(mBody);
}

void PirateShip::update()
{
    static const float outRight = mGraphicsManager.getRenderWidth() + mThisWidth;
    static const float outLeft = -mThisWidth;

    if (mLeftToRight)
    {
        PhysicsBody *body = mBody;
        if (body->location->x > outRight)
        {
            body->location->x = outLeft * RAND(3.0f);
        }
    }
    else
    {
        PhysicsBody *body = mBody;
        if (body->location->x < outLeft)
        {
            body->location->x = outRight * (1.0f + RAND(3.0f));
        }
    }
}

void PirateShip::setShipSpeed(float speed)
{
    mShipSpeed = speed;
    if (mLeftToRight) {
        mBody->velocityX = mShipSpeed;
    } else {
        mBody->velocityX = -mShipSpeed;
    }
}

void PirateShip::spawn(PhysicsBody* pBody)
{
    float posX, posY;

    if (mLeftToRight)
    {
        posX = -mThisWidth * RAND(3.0f);
        posY = mGraphicsManager.getRenderHeight() / 2.0f - mThisHeight / 2.0f;
        pBody->velocityX = mShipSpeed;
    }
    else
    {
        posX = mGraphicsManager.getRenderWidth() + mThisWidth;
        posY = mGraphicsManager.getRenderHeight() / 2.0f + mThisHeight / 2.0f;
        pBody->velocityX = -mShipSpeed;
    }

    pBody->velocityY = 0.0f;
    pBody->location->x = posX;
    pBody->location->y = posY;
}
