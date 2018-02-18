#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Background
{
public:
    Background(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager);
    void registerBackground(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

private:
    Background(const Background&);
    void operator=(const Background&);

    void spawn();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
};
