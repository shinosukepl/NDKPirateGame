#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Box
{
public:
    Box(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager);
    void registerBox(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

    inline PhysicsBody* getBody() const {return mBody;};

private:
    Box(const Box&);
    void operator=(const Box&);

    void spawn();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
    float mThisWidth;
    float mThisHeight;
};
