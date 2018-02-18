#include "Coins.h"

Coins::Coins(android_app* pApplication, TimeManager& pTimeManager,
             GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBodies(),
        mBodyCount(0),
        mCoinNum(0),
        mThisWidth(0.0f),
        mThisHeight(0.0f)
{

}

void Coins::registerCoins(Location& pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBodies[mBodyCount++] = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Coins::initialize()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        spawn(mBodies[i]);
    }
    mCoinNum = 0;
}

void Coins::update()
{
    for (int32_t i = 0; i < mBodyCount; ++i)
    {
        PhysicsBody* body = mBodies[i];
        spawn(body);
    }
}

void Coins::spawn(PhysicsBody* pBody)
{
    static const float startX = mGraphicsManager.getRenderWidth() / 2.0f + mThisWidth;
    static const float startY = mGraphicsManager.getRenderHeight() - mThisHeight;

    float posX = startX + mCoinNum * (mThisWidth * 1.2f);
    float posY = startY;


    pBody->velocityX = 0.0f;
    pBody->velocityY = 0.0f;
    pBody->location->x = posX;
    pBody->location->y = posY;

    mCoinNum++;
}