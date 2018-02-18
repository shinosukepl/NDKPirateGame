#pragma once

#include <android_native_app_glue.h>
#include "engine/TimeManager.hpp"
#include "engine/GraphicsManager.hpp"
#include "engine/PhysicsManager.hpp"

enum Slot
{
    LEFT,
    CENTER,
    RIGHT
};

enum Prize
{
    COIN,
    STAR,
    HEARTH,
    MAP
};

class Slots
{
public:
    Slots(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
          PhysicsManager& pPhysicsManager);

    void registerSlots(Location& pLocationLeft, Location& pLocationCenter,
                       Location& pLocationRight, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

private:
    Slots(const Slots&);
    void operator=(const Slots&);

    void spawn(PhysicsBody* pBodyLeft, PhysicsBody* pBodyCenter, PhysicsBody* pBodyRight);
    bool isTimePassed(double startTime, int seconds);
    bool stopOnPrize(PhysicsBody* pBody, int prize);
    bool updateEach(PhysicsBody* body, int32_t iter);
    void revealPrize();

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;

    PhysicsBody* mBodiesLeft[3];
    PhysicsBody* mBodiesCenter[3];
    PhysicsBody* mBodiesRight[3];
    int32_t mBodyCountLeft;
    int32_t mBodyCountCenter;
    int32_t mBodyCountRight;
    bool isLeftSlotLineStopped;
    bool isCenterSlotLineStopped;
    bool isRightSlotLineStopped;
    float mThisWidth;
    float mThisHeight;
    bool isPrizeRevealed;
    bool isStopAnimation;
};

