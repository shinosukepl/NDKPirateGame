#include "Cave.h"

Cave::Cave(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBodies(),
        mIsCaveFlashing(),
        mIsCoinCollected(),
        mBodyCount(0),
        mCaveNum(0),
        mThisWidth(0.0f),
        mThisHeight(0.0f)
{

}

void Cave::registerCave(Location& pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBodies[mBodyCount++] = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Cave::initialize()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        mIsCaveFlashing[i] = false;
        mIsCoinCollected[i] = false;
        spawn(mBodies[i]);
    }
    mCaveNum = 0;
}

void Cave::update()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        PhysicsBody* body = mBodies[i];
        spawn(body);
    }
}

void Cave::spawn(PhysicsBody* pBody)
{
    static const float renderHalfWidth = mThisWidth / 2.0f;
    static const float renderHalfHeight = mThisHeight / 2.0f;

    float posX = renderHalfWidth;
    float posY = renderHalfHeight;

    switch (mCaveNum)
    {
        case 0:
            posX = renderHalfWidth;
            posY = mGraphicsManager.getRenderHeight() - renderHalfHeight;
            break;
        case 1:
            posX = mGraphicsManager.getRenderWidth() - renderHalfWidth;
            posY = mGraphicsManager.getRenderHeight() - renderHalfHeight;
            break;
        case 2:
            posX = mGraphicsManager.getRenderWidth() - renderHalfWidth;
            posY = renderHalfHeight;
        default:
            break;
    }

    pBody->velocityX = 0.0f;
    pBody->velocityY = 0.0f;
    pBody->location->x = posX;
    pBody->location->y = posY;

    mCaveNum++;
}
