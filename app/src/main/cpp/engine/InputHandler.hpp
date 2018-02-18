#pragma once

#include <android/input.h>
#include <android/sensor.h>

class InputHandler
{
public:
    virtual ~InputHandler() {};

    virtual bool onTouchEvent(AInputEvent* pEvent) = 0;
};
