#pragma once

#include <android_native_app_glue.h>
#include "engine/ActivityHandler.hpp"
#include "engine/TimeManager.hpp"
#include "engine/GraphicsManager.hpp"
#include "engine/PhysicsManager.hpp"
#include "engine/InputManager.hpp"
#include "engine/SoundManager.hpp"
#include "engine/EventLoop.hpp"
#include "Cave.h"
#include "engine/SpriteBatch.hpp"
#include "engine/MoveableBody.hpp"
#include "pirate/PirateShip.h"
#include "Slots.h"
#include "Background.h"
#include "SlotMachine.h"
#include "Coin.h"
#include "Player.h"
#include "GameController.h"
#include "Coins.h"
#include "Lives.h"
#include "Box.h"
#include "PredefinedText.h"

class Pirate : public ActivityHandler
{
public:
    Pirate(android_app* pApplication);
    void run();

protected:
    status onActivate();
    void onDeactivate();
    status onStep();

    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();

    void onSaveInstanceState(void** pData, size_t* pSize);
    void onConfigurationChanged();
    void onLowMemory();

    void onCreateWindow();
    void onDestroyWindow();
    void onGainFocus();
    void onLostFocus();

private:
    Pirate(const Pirate&);
    void operator=(const Pirate&);

    status onGameStep();
    status onStartGameStep();
    status onTutorialStep();
    status onGameOverStep();
    status onLevelUpStep();

    void onShipCollision();
    void onCaveCollision();
    void onStartPointCollision();

    TimeManager     mTimeManager;
    GraphicsManager mGraphicsManager;
    PhysicsManager  mPhysicsManager;
    SoundManager    mSoundManager;
    InputManager    mInputManager;
    EventLoop mEventLoop;
    GameController mGameController;

    Resource mBGM;
    Resource mCoinSound;
    Resource mHitSound;
    Resource mTextSound;

    Resource mBackgroundTexture;
    Resource mCaveTexture;
    Resource mSlotsTexture;
    Resource mSlotMachineTexture;
    Resource mShipTexture;
    Resource mCoinTexture;
    Resource mPlayerTexture;
    Resource mCoinsTexture;
    Resource mLivesTexture;
    Resource mBoxTexture;
    Resource mStartTextTexture;
    Resource mLevelUpTextTexture;
    Resource mGameOverTextTexture;

    Sound* mCoinPlaySound;
    Sound* mHitPlaySound;
    Sound* mTextPlaySound;

    Background mBackground;
    Cave mCaves;
    Slots mSlots;
    SlotMachine mSlotMachine;
    PirateShip mShipBrown;
    PirateShip mShipRed;
    Coin mCoin;
    Player mPlayer;
    Coins mCoins;
    Lives mLives;
    Box mBox;
    PredefinedText mStartText;
    PredefinedText mLevelUpText;
    PredefinedText mGameOverText;

    SpriteBatch mSpriteBatch;
    MoveableBody mMoveableBody;

    static const int NUMBER_OF_CAVES = 3;
    static const int BACKGROUND_TEXTURE_WIDTH = 640;
    static const int BACKGROUND_TEXTURE_HEIGHT = 384;

};
