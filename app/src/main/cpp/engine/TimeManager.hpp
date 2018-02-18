#pragma once

#include "Types.hpp"

class TimeManager
{
public:
    TimeManager();

    void reset();
    void update();
    double now();

    inline float elapsed() const { return mElapsed; };
    inline float elapsedTotal() const { return mElapsedTotal; };

private:
    TimeManager(const TimeManager&);
    void operator=(const TimeManager&);

    double mFirstTime;
    double mLastTime;
    float mElapsed;
    float mElapsedTotal;
};
