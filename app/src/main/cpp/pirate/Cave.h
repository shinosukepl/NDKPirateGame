#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/TimeManager.hpp"
#include "engine/PhysicsManager.hpp"

class Cave
{
public:
    Cave(android_app* pApplication, TimeManager& pTimeManager, GraphicsManager& pGraphicsManager,
         PhysicsManager& pPhysicsManager);

    void registerCave(Location& pLocation, int32_t pSizeX, int32_t pSizeY);
    void initialize();
    void update();

    inline PhysicsBody* getBody(int id) const {return mBodies[id];};
    inline bool getCaveFlashing(int id) const {return mIsCaveFlashing[id];};
    inline void setCaveFlashing(int id, bool isFlashing) {mIsCaveFlashing[id] = isFlashing;};

private:
    Cave(const Cave&);
    void operator=(const Cave&);

    void spawn(PhysicsBody* pBody);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;
    PhysicsBody* mBodies[3];
    bool mIsCaveFlashing[3];
    bool mIsCoinCollected[3];
    int32_t mBodyCount;
    int32_t mCaveNum;
    float mThisWidth;
    float mThisHeight;
};
