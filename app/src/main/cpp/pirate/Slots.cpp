#include "Slots.h"
#include "engine/Log.hpp"

Slots::Slots(android_app* pApplication, TimeManager& pTimeManager,
             GraphicsManager& pGraphicsManager, PhysicsManager& pPhysicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsManager(pPhysicsManager),
        mBodiesLeft(),
        mBodiesCenter(),
        mBodiesRight(),
        mBodyCountLeft(0),
        mBodyCountCenter(0),
        mBodyCountRight(0),
        mThisWidth(0.0f),
        mThisHeight(0.0f),
        isPrizeRevealed(false),
        isStopAnimation(false),
        isLeftSlotLineStopped(false),
        isCenterSlotLineStopped(false),
        isRightSlotLineStopped(false)
{

}

void Slots::registerSlots(Location& pLocationLeft, Location& pLocationCenter,
                          Location& pLocationRight, int32_t pSizeX, int32_t pSizeY)
{
    mBodiesLeft[mBodyCountLeft++] = mPhysicsManager.loadBody(pLocationLeft, pSizeX, pSizeY);
    mBodiesCenter[mBodyCountCenter++] = mPhysicsManager.loadBody(pLocationCenter, pSizeX, pSizeY);
    mBodiesRight[mBodyCountRight++] = mPhysicsManager.loadBody(pLocationRight, pSizeX, pSizeY);
    mThisWidth = (float)pSizeX;
    mThisHeight = (float)pSizeY;
}

void Slots::initialize()
{
    for (int32_t i = 0; i < 3; ++i)
    {
        spawn(mBodiesLeft[i], mBodiesCenter[i], mBodiesRight[i]);
    }
}

bool Slots::updateEach(PhysicsBody* body, int32_t iter)
{
    static const float BOUND = mGraphicsManager.getRenderHeight() / 2.0f - mThisHeight;
    static const float START_POSITION = mGraphicsManager.getRenderHeight() / 2.0f + 2 * mThisHeight;

    if (body->location->y < BOUND)
    {
        body->location->y = START_POSITION;
        if (iter == 0) return true;
    }

    return false;
}

void Slots::update()
{
    Log::info("LUK LEFT Y: %f", mBodiesLeft[0]->location->y);
    Log::info("LUK CENTER Y: %f", mBodiesCenter[0]->location->y);
    Log::info("LUK RIGHT Y: %f", mBodiesRight[0]->location->y);

    static bool isTimeToStop = false;

    if (isPrizeRevealed) return;

    for (int32_t iter = 0; iter < 3; ++iter)
    {
        if (!isLeftSlotLineStopped)
        {
            if (updateEach(mBodiesLeft[iter], iter) && isTimeToStop) isStopAnimation = true;
        }

        if (!isCenterSlotLineStopped)
        {
            if (updateEach(mBodiesCenter[iter], iter) && isTimeToStop) isStopAnimation = true;
        }

        if (!isRightSlotLineStopped)
        {
            if (updateEach(mBodiesRight[iter], iter) && isTimeToStop) isStopAnimation = true;
        }

        if (isStopAnimation) revealPrize();

        static double startTime = mTimeManager.now();
        if(isTimePassed(startTime, 3))
        {
            isTimeToStop = true;
        }
    }
}

void Slots::revealPrize()
{
    static const int TIME_BETWEEN_STOPS = 1;
    static const int PRIZE = 2;
    static Slot state = LEFT;

    static double startTime = mTimeManager.now();
    if(isTimePassed(startTime, TIME_BETWEEN_STOPS))
    {
        switch (state)
        {
            case LEFT:
                if (!isLeftSlotLineStopped && stopOnPrize(mBodiesLeft[0], PRIZE))
                {
                    mBodiesLeft[1]->velocityY = 0.0f;
                    mBodiesLeft[2]->velocityY = 0.0f;
                    isLeftSlotLineStopped = true;
                    isStopAnimation = false;
                    state = CENTER;
                    startTime = mTimeManager.now();
                }
                break;
            case CENTER:
                if (!isCenterSlotLineStopped && stopOnPrize(mBodiesCenter[0], PRIZE))
                {
                    mBodiesCenter[1]->velocityY = 0.0f;
                    mBodiesCenter[2]->velocityY = 0.0f;
                    isCenterSlotLineStopped = true;
                    isStopAnimation = false;
                    state = RIGHT;
                    startTime = mTimeManager.now();
                }
                break;
            case RIGHT:
                if (!isRightSlotLineStopped && stopOnPrize(mBodiesRight[0], PRIZE))
                {
                    mBodiesRight[1]->velocityY = 0.0f;
                    mBodiesRight[2]->velocityY = 0.0f;
                    isRightSlotLineStopped = true;
                    isPrizeRevealed = true;
                }
                break;
            default:
                break;
        }
    }
}

bool Slots::stopOnPrize(PhysicsBody* pBody, int prize)
{
    static const float HALF_OF_SINGLE_PRIZE = mThisHeight / 8.0f;
    static const float SINGLE_PRIZE = mThisHeight / 4.0f;
    static const float WINNER = SINGLE_PRIZE * prize + HALF_OF_SINGLE_PRIZE;

    if ((pBody->location->y + WINNER) < (mGraphicsManager.getRenderHeight() / 2.0f))
    {
        pBody->velocityY = 0.0f;
        pBody->location->y = mGraphicsManager.getRenderHeight() / 2.0f - WINNER;
        Log::info("LUK LOCATION PICKED - V: %f, Y: %f", pBody->velocityY, pBody->location->y);
        return true;
    }

    return false;
}

bool Slots::isTimePassed(double startTime, int seconds)
{
    return mTimeManager.now() - startTime > seconds;
}

void Slots::spawn(PhysicsBody* pBodyLeft, PhysicsBody* pBodyCenter, PhysicsBody* pBodyRight)
{
    static const float THIS_HALF_WIDTH = mThisWidth / 2.0f;
    static const float RENDER_HALF_WIDTH = mGraphicsManager.getRenderWidth() / 2.0f;
    static const float RENDER_HALF_HEIGHT = mGraphicsManager.getRenderHeight() / 2.0f;
    static const float SLOT_SPEED = -1000.0f;

    static int slotNum = 0;

    float posXLeft = RENDER_HALF_WIDTH - 2 * THIS_HALF_WIDTH;
    float posXCenter = RENDER_HALF_WIDTH;
    float posXRight = RENDER_HALF_WIDTH + 2 * THIS_HALF_WIDTH;

    float posHeight = RENDER_HALF_HEIGHT;

    switch (slotNum)
    {
        case 0:
            posHeight = RENDER_HALF_HEIGHT;
            break;
        case 1:
            posHeight = RENDER_HALF_HEIGHT + mThisHeight;
            break;
        case 2:
            posHeight = RENDER_HALF_HEIGHT + 2 * mThisHeight;
        default:
            break;
    }

    pBodyLeft->velocityX = 0.0f;
    pBodyLeft->velocityY = SLOT_SPEED;
    pBodyLeft->location->x = posXLeft;
    pBodyLeft->location->y = posHeight;

    pBodyCenter->velocityX = 0.0f;
    pBodyCenter->velocityY = SLOT_SPEED;
    pBodyCenter->location->x = posXCenter;
    pBodyCenter->location->y = posHeight;

    pBodyRight->velocityX = 0.0f;
    pBodyRight->velocityY = SLOT_SPEED;
    pBodyRight->location->x = posXRight;
    pBodyRight->location->y = posHeight;

    slotNum++;
}
