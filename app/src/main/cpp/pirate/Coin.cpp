#include "Coin.h"

Coin::Coin(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
           PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBody(),
        mThisWidth(0.0f),
        mThisHeight(0.0f),
        mStartY(0.0f)
{

}

void Coin::registerCoin(Location &pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Coin::setLocation(Location location)
{
    mBody->location->x = location.x;
    mBody->location->y = location.y;
    mStartY = location.y;
};

void Coin::initialize()
{
    mStartY = mGraphicsManager.getRenderHeight() - 4 * mThisHeight;
    spawn();
}

void Coin::update()
{
    if (mBody->location->y > mStartY + 4 * mThisHeight)
        mBody->location->y = mStartY;
}

void Coin::spawn()
{
    float posX = 4 * mThisWidth;
    float posY = mStartY;

    mBody->velocityX = 0.0f;
    mBody->velocityY = 65.0f;
    mBody->location->x = posX;
    mBody->location->y = posY;
}
