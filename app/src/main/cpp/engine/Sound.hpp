#pragma once

class SoundManager;

#include "Resource.hpp"
#include "Types.hpp"

class Sound
{
public:
    Sound(android_app* pApplication, Resource* pResource);

    const char* getPath();
    inline uint8_t* getBuffer() const { return mBuffer; };
    inline off_t getLength() const { return mLength; };

    status load();
    status loadError();
    status unload();

private:
    Sound(const Sound&);
    void operator=(const Sound&);
    friend class SoundManager;

    Resource* mResource;
    uint8_t* mBuffer;
    off_t mLength;
};
