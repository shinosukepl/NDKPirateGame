#pragma once

#include "GraphicsManager.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

struct PhysicsBody
{
    PhysicsBody(Location* pLocation, int32_t pWidth, int32_t pHeight):
        location(pLocation),
        width(pWidth), height(pHeight),
        velocityX(0.0f), velocityY(0.0f),
        radius(0)
    {
        //
    }

    Location* location;
    int32_t width;
    int32_t height;
    int32_t radius;
    float velocityX;
    float velocityY;
};

class PhysicsManager
{
public:
    PhysicsManager(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager);
    ~PhysicsManager();

    PhysicsBody* loadBody(Location& pLocation, int32_t pWidth, int32_t pHeight);
    void update();

private:
    PhysicsManager(const PhysicsManager&);
    void operator=(const PhysicsManager&);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;

    // TODO: change to dynamic
    PhysicsBody* mPhysicsBodies[1024];
    int32_t mPhysicsBodyCount;
};
