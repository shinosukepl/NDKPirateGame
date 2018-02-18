#include "Background.h"

Background::Background(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBody()
{

}

void Background::registerBackground(Location &pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
}

void Background::initialize()
{
    spawn();
}

void Background::update()
{
//
}

void Background::spawn()
{
    float posX = mGraphicsManager.getRenderWidth() / 2.0f;
    float posY = mGraphicsManager.getRenderHeight() / 2.0f;

    mBody->velocityX = 0.0f;
    mBody->velocityY = 0.0f;
    mBody->location->x = posX;
    mBody->location->y = posY;
}
