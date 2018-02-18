#include "Log.hpp"
#include "Sound.hpp"
#include <SLES/OpenSLES_Android.h>

Sound::Sound(android_app* pApplication, Resource* pResource) :
    mResource(pResource),
    mBuffer(NULL),
    mLength(0)
{
    //
}

const char* Sound::getPath()
{
    return mResource->getPath();
}

status Sound::load()
{
    Log::info("Loading sound %s", mResource->getPath());
    status result;

    // Opens sound file.
    if (mResource->open() != STATUS_OK)
    {
        return loadError();
    }

    // Reads sound file.
    mLength = mResource->getLength();
    mBuffer = new uint8_t[mLength];
    result = mResource->read(mBuffer, mLength);
    mResource->close();
    return STATUS_OK;
}

status Sound::loadError()
{
    Log::error("Error while reading PCM sound.");
    return STATUS_KO;
}

status Sound::unload()
{
    delete[] mBuffer;
    mBuffer = NULL; mLength = 0;

    return STATUS_OK;
}
