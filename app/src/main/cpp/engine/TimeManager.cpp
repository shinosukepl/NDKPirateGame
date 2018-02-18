#include "Log.hpp"
#include "TimeManager.hpp"
#include <cstdlib>
#include <ctime>

TimeManager::TimeManager():
    mFirstTime(0.0f),
    mLastTime(0.0f),
    mElapsed(0.0f),
    mElapsedTotal(0.0f)
{
    srand(time(NULL));
}

void TimeManager::reset()
{
    Log::info("Resetting TimeManager.");
    mElapsed = 0.0f;
    mFirstTime = now();
    mLastTime = mFirstTime;
}

void TimeManager::update()
{
    // Checks elapsed time since last frame. It is important to
    // work on double with current time to avoid losing accuracy
    // Then we can go back to float for elapsed time.
    double currentTime = now();
    mElapsed = (currentTime - mLastTime);
    mElapsedTotal = (currentTime - mFirstTime);
    mLastTime = currentTime;
}

double TimeManager::now()
{
    // Retrieves current time with a monotonic clock to ensure
    // time goes forward and is not subject to system changes.
    timespec timeVal;
    clock_gettime(CLOCK_MONOTONIC, &timeVal);
    return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
}
