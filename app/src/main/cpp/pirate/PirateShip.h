#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/PhysicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/Types.hpp"

class PirateShip
{
public:
    PirateShip(android_app* pApplication, TimeManager& pTimeManager,
               GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager);

    void registerShip(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize(bool leftToRight);
    void update();

    void setShipSpeed(float speed);

    inline PhysicsBody* getBody() const {return mBody;};
    inline float getShipSpeed() const {return mShipSpeed;};

private:
    PirateShip(const PirateShip&);
    void operator=(const PirateShip&);

    void spawn(PhysicsBody* pBody);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
    bool mLeftToRight;
    float mThisWidth;
    float mThisHeight;
    float mShipSpeed;
};

