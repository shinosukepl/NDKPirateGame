#pragma once

#include "GraphicsManager.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"
#include <android_native_app_glue.h>

class InputManager : public InputHandler
{
public:
    InputManager(android_app* pApplication, GraphicsManager& pGraphicsManager);

    inline float getDirectionX() const { return mDirectionX; };
    inline float getDirectionY() const { return mDirectionY; };
    inline bool getTouched() const {return mScreenHasBeenTouched;};
    inline void setRefPoint(Location* pRefPoint) { mRefPoint = pRefPoint; };

    void start();

protected:
    bool onTouchEvent(AInputEvent* pEvent);

private:
    InputManager(const InputManager&);
    void operator=(const InputManager&);

    android_app* mApplication;
    GraphicsManager& mGraphicsManager;

    // Input values.
    float mScaleFactor;
    float mDirectionX, mDirectionY;

    bool mScreenHasBeenTouched;

    // Reference point to evaluate touch distance.
    Location* mRefPoint;
};
