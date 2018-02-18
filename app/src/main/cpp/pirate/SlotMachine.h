#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class SlotMachine
{
public:
    SlotMachine(android_app* pApplication, TimeManager& pTimeManager,
                GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager);

    void registerSlotMachine(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

private:
    SlotMachine(const SlotMachine&);
    void operator=(const SlotMachine&);

    void spawn();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
};
