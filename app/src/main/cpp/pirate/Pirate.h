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

/*
        Sprite *caveGraphics = mSpriteBatch.registerSprite(mCaveTexture, 128, 64);
        caveGraphics->setAnimation(0, 1, 0.0f, false);
        switch (i)
        {
            case 0:
                caveGraphics->setId(mGameController.CAVE_TOP_LEFT_ID);
                break;
            case 1:
                caveGraphics->setId(mGameController.CAVE_TOP_RIGHT_ID);
                break;
            case 2:
                caveGraphics->setId(mGameController.CAVE_BOTTOM_RIGHT_ID);
                break;
            default:
                break;
        }

        mCaves.registerCave(caveGraphics->location, 128, 64);
    }

//    for (int32_t i = 0; i < 3; ++i)
//    {
//        Sprite* slotsGraphicsLeft = mSpriteBatch.registerSprite(mSlotsTexture, 128, 512);
//        Sprite* slotsGraphicsCenter = mSpriteBatch.registerSprite(mSlotsTexture, 128, 512);
//        Sprite* slotsGraphicsRight = mSpriteBatch.registerSprite(mSlotsTexture, 128, 512);
//        mSlots.registerSlots(slotsGraphicsLeft->location, slotsGraphicsCenter->location, slotsGraphicsRight->location, 128, 512);
//    }

    //Sprite *slotMachineGraphics = mSpriteBatch.registerSprite(mSlotMachineTexture, 512, 384);
    //mSlotMachine.registerSlotMachine(slotMachineGraphics->location, 512, 384);


    Sprite* shipBrownGraphics = mSpriteBatch.registerSprite(mShipTexture, 128, 64);
    float animShipSpeed = 8.0f;
    shipBrownGraphics->setAnimation(1, 2, animShipSpeed, true);
    shipBrownGraphics->setId(mGameController.PIRATE_SHIP_BROWN_ID);
    mShipBrown.registerShip(shipBrownGraphics->location, 128, 64);

    Sprite* shipRedGraphics = mSpriteBatch.registerSprite(mShipTexture, 128, 64);
    shipRedGraphics->setAnimation(9, 2, animShipSpeed, true);
    shipRedGraphics->setId(mGameController.PIRATE_SHIP_RED_ID);
    mShipRed.registerShip(shipRedGraphics->location, 128, 64);

    Sprite* coinGraphics = mSpriteBatch.registerSprite(mCoinTexture, 16, 16);
    mGraphicsManager.setSpriteAnimation(mGameController.COIN_ANIM_ID , 0, 1, 0.0f, false);
    coinGraphics->setId(mGameController.COIN_ANIM_ID);
    mCoin.registerCoin(coinGraphics->location, 16, 16);

    Sprite* boxGraphics = mSpriteBatch.registerSprite(mBoxTexture, 48, 48);
    boxGraphics->setAnimation(0, 1, 0.0f, false);
    boxGraphics->setId(mGameController.BOX_ID);
    mBox.registerBox(boxGraphics->location, 48, 48);

    Sprite* playerGraphics = mSpriteBatch.registerSprite(mPlayerTexture, 48, 48);
    playerGraphics->setAnimation(0, 1, 0.0f, false);
    playerGraphics->setId(mGameController.PLAYER_ID);
    mMoveableBody.registerMoveableBody(playerGraphics->location, 48, 48);
    mPlayer.registerPlayer(playerGraphics, mCoinPlaySound);

    for (int32_t i = 0; i < 3; ++i)
    {
        Sprite *coinsGraphics = mSpriteBatch.registerSprite(mCoinsTexture, 16, 16);
        coinsGraphics->setId(mGameController.START_COIN_ID + i);
        mCoins.registerCoins(coinsGraphics->location, 16, 16);
        mGraphicsManager.setSpriteEnabled(mGameController.START_COIN_ID + i, false);

        Sprite *livesGraphics = mSpriteBatch.registerSprite(mLivesTexture, 16, 16);
        livesGraphics->setId(mGameController.START_LIVES_ID + i);
        mLives.registerLives(livesGraphics->location, 16, 16);
    }

    Sprite *startTextGraphics = mSpriteBatch.registerSprite(mStartTextTexture, 320, 48);
    startTextGraphics->setId(mGameController.TEXT_START_ID);
    mStartText.registerPredefinedText(startTextGraphics->location, 320, 48);

    Sprite *levelUpTextGraphics = mSpriteBatch.registerSprite(mLevelUpTextTexture, 192, 48);
    levelUpTextGraphics->setId(mGameController.TEXT_LEVEL_UP_ID);
    mLevelUpText.registerPredefinedText(levelUpTextGraphics->location, 192, 48);

    Sprite *gameOverTextGraphics = mSpriteBatch.registerSprite(mGameOverTextTexture, 256, 48);
    gameOverTextGraphics->setId(mGameController.TEXT_GAME_OVER_ID);
    mGameOverText.registerPredefinedText(gameOverTextGraphics->location, 256, 48);
    */
