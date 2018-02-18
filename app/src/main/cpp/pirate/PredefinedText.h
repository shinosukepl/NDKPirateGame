#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class PredefinedText
{
public:
    PredefinedText(android_app* pApplication, TimeManager& pTimeManager,
                   GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager);

    void registerPredefinedText(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

    inline PhysicsBody* getBody() const {return mBody;};

private:
    PredefinedText(const PredefinedText&);
    void operator=(const PredefinedText&);

    void spawn();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBody;
    float mThisWidth;
    float mThisHeight;
};
