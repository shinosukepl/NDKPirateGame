#pragma once

#include "Types.hpp"
#include <android_native_app_glue.h>

struct ResourceDescriptor
{
    int32_t mDescriptor;
    off_t mStart;
    off_t mLength;
};

class Resource
{
public:
    Resource(android_app* pApplication, const char* pPath);
    inline const char* getPath() const { return mPath; };
    status open();
    void close();
    status read(void* pBuffer, size_t pCount);

    ResourceDescriptor descriptor();
    off_t getLength();

    bool operator==(const Resource& pOther);

private:
    Resource(const Resource&);
    void operator=(const Resource&);

    const char* mPath;
    AAssetManager* mAssetManager;
    AAsset* mAsset;
};
