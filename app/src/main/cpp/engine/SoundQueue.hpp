#pragma once

#include "Sound.hpp"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundQueue
{
public:
    SoundQueue();

    status initialize(SLEngineItf pEngine, SLObjectItf pOutputMixObj);
    status initializeError();
    void finalize();
    void playSound(Sound* pSound);
    void playSoundError();

private:
    SoundQueue(const SoundQueue&);
    void operator=(const SoundQueue&);

    // Sound player.
    SLObjectItf mPlayerObj;
    SLPlayItf mPlayer;
    SLBufferQueueItf mPlayerQueue;
};
