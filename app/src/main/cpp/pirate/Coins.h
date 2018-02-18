#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Coins
{
public:
    Coins(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
          PhysicsManager& pPhysicsManager);

    void registerCoins(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

private:
    Coins(const Coins&);
    void operator=(const Coins&);

    void spawn(PhysicsBody* pBody);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBodies[3];
    int32_t mBodyCount;
    int32_t mCoinNum;
    float mThisWidth;
    float mThisHeight;
};
