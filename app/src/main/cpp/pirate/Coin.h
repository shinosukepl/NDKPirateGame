#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Coin
{
public:
    Coin(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
         PhysicsManager& pPhysicsManager);

    void registerCoin(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();
    void setLocation(Location location);

private:
    Coin(const Coin&);
    void operator=(const Coin&);

    void spawn();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
    float mThisWidth;
    float mThisHeight;
    float mStartY;
};
