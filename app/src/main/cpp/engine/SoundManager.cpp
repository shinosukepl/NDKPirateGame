#include <cstring>
#include "Log.hpp"
#include "Resource.hpp"
#include "SoundManager.hpp"

SoundManager::SoundManager(android_app* pApplication) :
    mApplication(pApplication),
    mEngineObj(NULL),
    mEngine(NULL),
    mOutputMixObj(NULL),
    mBGMPlayerObj(NULL),
    mBGMPlayer(NULL),
    mBGMPlayerSeek(NULL),
    mSoundQueues(),
    mCurrentQueue(0),
    mSounds(),
    mSoundCount(0)
{
    Log::info("Creating SoundManager.");
}

SoundManager::~SoundManager()
{
    Log::info("Destroying SoundManager.");
    for (int32_t i = 0; i < mSoundCount; ++i)
    {
        delete mSounds[i];
    }
    mSoundCount = 0;
}

status SoundManager::start()
{
    Log::info("Starting SoundManager.");
    SLresult result;
    const SLuint32 engineMixIIDCount = 1;
    const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
    const SLboolean engineMixReqs[] = {SL_BOOLEAN_TRUE};
    const SLuint32 outputMixIIDCount = 0;
    const SLInterfaceID outputMixIIDs[] = {};
    const SLboolean outputMixReqs[] = {};

    // Creates OpenSL ES engine and dumps its capabilities.
    result = slCreateEngine(&mEngineObj, 0, NULL, engineMixIIDCount, engineMixIIDs, engineMixReqs);
    if (result != SL_RESULT_SUCCESS) return startError();
    result = (*mEngineObj)->Realize(mEngineObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) return startError();
    result = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);
    if (result != SL_RESULT_SUCCESS) return startError();

    // Creates audio output.
    result = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj, outputMixIIDCount,
                                         outputMixIIDs, outputMixReqs);
    result = (*mOutputMixObj)->Realize(mOutputMixObj, SL_BOOLEAN_FALSE);

    // Set-up sound player.
    Log::info("Starting sound player.");
    for (int32_t i = 0; i < QUEUE_COUNT; ++i)
    {
        if (mSoundQueues[i].initialize(mEngine, mOutputMixObj) != STATUS_OK)
            return startError();
    }

    // Loads resources
    for (int32_t i = 0; i < mSoundCount; ++i)
    {
        if (mSounds[i]->load() != STATUS_OK) return startError();
    }
    return STATUS_OK;
}

status SoundManager::startError()
{
    Log::error("Error while starting SoundManager");
    stop();
    return STATUS_KO;
}

void SoundManager::stop()
{
    Log::info("Stopping SoundManager.");

    // Stops and destroys BGM player.
    stopBGM();

    // Destroys sound player.
    for (int32_t i= 0; i < QUEUE_COUNT; ++i)
    {
        mSoundQueues[i].finalize();
    }

    // Destroys audio output and engine.
    if (mOutputMixObj != NULL)
    {
        (*mOutputMixObj)->Destroy(mOutputMixObj);
        mOutputMixObj = NULL;
    }
    if (mEngineObj != NULL)
    {
        (*mEngineObj)->Destroy(mEngineObj);
        mEngineObj = NULL;
        mEngine = NULL;
    }

    // Frees sound resources.
    for (int32_t i = 0; i < mSoundCount; ++i)
    {
        mSounds[i]->unload();
    }
}

status SoundManager::playBGM(Resource& pResource)
{
    SLresult result;
    Log::info("Opening BGM %s", pResource.getPath());

    // Set-up BGM audio source.
    ResourceDescriptor descriptor = pResource.descriptor();
    if (descriptor.mDescriptor < 0)
    {
        Log::info("Could not open BGM file");
        return STATUS_KO;
    }

    SLDataLocator_AndroidFD dataLocatorIn;
    dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
    dataLocatorIn.fd = descriptor.mDescriptor;
    dataLocatorIn.offset = descriptor.mStart;
    dataLocatorIn.length = descriptor.mLength;

    SLDataFormat_MIME dataFormat;
    dataFormat.formatType = SL_DATAFORMAT_MIME;
    dataFormat.mimeType = NULL;
    dataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

    SLDataSource dataSource;
    dataSource.pLocator = &dataLocatorIn;
    dataSource.pFormat = &dataFormat;

    SLDataLocator_OutputMix dataLocatorOut;
    dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    dataLocatorOut.outputMix = mOutputMixObj;

    SLDataSink dataSink;
    dataSink.pLocator = &dataLocatorOut;
    dataSink.pFormat = NULL;

    // Creates BGM player and retrieves its interfaces.
    const SLuint32 bgmPlayerIIDCount = 2;
    const SLInterfaceID bgmPlayerIIDs[] = {SL_IID_PLAY, SL_IID_SEEK};
    const SLboolean bgmPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*mEngine)->CreateAudioPlayer(mEngine, &mBGMPlayerObj, &dataSource, &dataSink,
                                           bgmPlayerIIDCount, bgmPlayerIIDs, bgmPlayerReqs);
    if (result != SL_RESULT_SUCCESS) return playBGMError();
    result = (*mBGMPlayerObj)->Realize(mBGMPlayerObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) return playBGMError();

    result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_PLAY, &mBGMPlayer);
    if (result != SL_RESULT_SUCCESS) return playBGMError();
    result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_SEEK, &mBGMPlayerSeek);
    if (result != SL_RESULT_SUCCESS) return playBGMError();

    // Enables looping and starts playing.
    result = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    if (result != SL_RESULT_SUCCESS) return playBGMError();
    result = (*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS) return playBGMError();

    return STATUS_OK;
}

status SoundManager::playBGMError()
{
    Log::error("Error playing BGM");
    return STATUS_KO;
}

void SoundManager::stopBGM()
{
    if (mBGMPlayer != NULL)
    {
        SLuint32 bgmPlayerState;
        (*mBGMPlayerObj)->GetState(mBGMPlayerObj, &bgmPlayerState);
        if (bgmPlayerState == SL_OBJECT_STATE_REALIZED)
        {
            (*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PAUSED);

            (*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
            mBGMPlayerObj = NULL;
            mBGMPlayer = NULL;
            mBGMPlayerSeek = NULL;
        }
    }
}

Sound* SoundManager::registerSound(Resource& pResource)
{
    for (int32_t i = 0; i < mSoundCount; ++i)
    {
        if (strcmp(pResource.getPath(), mSounds[i]->getPath()) == 0)
        {
            return mSounds[i];
        }
    }

    Sound* sound = new Sound(mApplication, &pResource);
    mSounds[mSoundCount++] = sound;
    return sound;
}

void SoundManager::playSound(Sound* pSound)
{
    int32_t currentQueue = ++mCurrentQueue;
    SoundQueue& soundQueue = mSoundQueues[currentQueue % QUEUE_COUNT];
    soundQueue.playSound(pSound);
}
