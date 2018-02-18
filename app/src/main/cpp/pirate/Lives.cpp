#include "Lives.h"

Lives::Lives(android_app* pApplication, TimeManager& pTimeManager,
             GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBodies(),
        mBodyCount(0),
        mLiveNum(0),
        mThisWidth(0.0f),
        mThisHeight(0.0f)
{

}

void Lives::registerLives(Location &pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBodies[mBodyCount++] = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Lives::initialize()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        spawn(mBodies[i]);
    }
    mLiveNum = 0;
}

void Lives::update()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        PhysicsBody* body = mBodies[i];
        spawn(body);
    }
}

void Lives::spawn(PhysicsBody* pBody)
{
    static const float startX = mGraphicsManager.getRenderWidth() / 2.0f - 4 * mThisWidth;
    static const float startY = mGraphicsManager.getRenderHeight() - mThisHeight;

    float posX = startX + mLiveNum * (mThisWidth * 1.2f);
    float posY = startY;


    pBody->velocityX = 0.0f;
    pBody->velocityY = 0.0f;
    pBody->location->x = posX;
    pBody->location->y = posY;

    mLiveNum++;
}
