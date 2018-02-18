#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Lives
{
public:
    Lives(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
          PhysicsManager& pPhysicsManager);

    void registerLives(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

private:
    Lives(const Lives&);
    void operator=(const Lives&);

    void spawn(PhysicsBody* pBody);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBodies[3];
    int32_t mBodyCount;
    int32_t mLiveNum;
    float mThisWidth;
    float mThisHeight;
};
