#include "InputManager.hpp"
#include "Log.hpp"
#include <android_native_app_glue.h>

InputManager::InputManager(android_app* pApplication, GraphicsManager& pGraphicsManager) :
    mApplication(pApplication),
    mGraphicsManager(pGraphicsManager),
    mDirectionX(0.0f),
    mDirectionY(0.0f),
    mScreenHasBeenTouched(false),
    mRefPoint(NULL)
{
    //
}

void InputManager::start()
{
    Log::info("Starting InputManager.");
    mDirectionX = 0.0f, mDirectionY = 0.0f;
    mScaleFactor = float(mGraphicsManager.getRenderWidth()) / float(mGraphicsManager.getScreenWidth());
}

bool InputManager::onTouchEvent(AInputEvent* pEvent)
{
    static const float TOUCH_MAX_RANGE = 65.0f; // In game units.

    if (mRefPoint != NULL) {
        if (AMotionEvent_getAction(pEvent)
                        == AMOTION_EVENT_ACTION_MOVE) {
            float x = AMotionEvent_getX(pEvent, 0) * mScaleFactor;
            float y = (float(mGraphicsManager.getScreenHeight())
                     - AMotionEvent_getY(pEvent, 0)) * mScaleFactor;
            // Needs a conversion to proper coordinates
            // (origin at bottom/left). Only moveY needs it.
            float moveX = x - mRefPoint->x;
            float moveY = y - mRefPoint->y;
            float moveRange = sqrt((moveX * moveX) + (moveY * moveY));

            if (moveRange > TOUCH_MAX_RANGE) {
                float cropFactor = TOUCH_MAX_RANGE / moveRange;
                moveX *= cropFactor; moveY *= cropFactor;
            }

            mDirectionX = moveX / TOUCH_MAX_RANGE;
            mDirectionY   = moveY / TOUCH_MAX_RANGE;
            mScreenHasBeenTouched = true;
        } else {
            mDirectionX = 0.0f; mDirectionY = 0.0f;
            mScreenHasBeenTouched = false;
        }
    }
    return true;
}
