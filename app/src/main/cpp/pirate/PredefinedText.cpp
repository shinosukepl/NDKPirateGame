#include "PredefinedText.h"

PredefinedText::PredefinedText(android_app* pApplication, TimeManager& pTimeManager,
                               GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBody(),
        mThisWidth(0.0f),
        mThisHeight(0.0f)
{

}

void PredefinedText::registerPredefinedText(Location &pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void PredefinedText::initialize()
{
    spawn();
}

void PredefinedText::update()
{

}

void PredefinedText::spawn()
{
    float posX = mGraphicsManager.getRenderWidth() / 2.0f;
    float posY = mGraphicsManager.getRenderHeight() / 2.0f;

    mBody->velocityX = 0.0f;
    mBody->velocityY = 0.0f;
    mBody->location->x = posX;
    mBody->location->y = posY;
}