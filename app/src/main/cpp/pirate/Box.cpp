#include "Box.h"

Box::Box(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBody(),
        mThisWidth(0.0f),
        mThisHeight(0.0f)
{

}

void Box::registerBox(Location &pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Box::initialize()
{
    spawn();
}

void Box::update()
{
}

void Box::spawn()
{
    float posX = mThisWidth * 2.0f / 3.0f;
    float posY = mThisHeight * 2.0f / 3.0f;

    mBody->velocityX = 0.0f;
    mBody->velocityY = 0.0f;
    mBody->location->x = posX;
    mBody->location->y = posY;
}
