#include "Pirate.h"
#include "engine/Log.hpp"

Pirate::Pirate(android_app* pApplication):
        mTimeManager(),
        mGraphicsManager(pApplication),
        mPhysicsManager(mTimeManager, mGraphicsManager),
        mSoundManager(pApplication),
        mInputManager(pApplication, mGraphicsManager),
        mEventLoop(pApplication, *this, mInputManager),
        mGameController(),

        mBGM(pApplication, "pirate/Mister-Sneakypants_Looping.mp3"),
        mCoinSound(pApplication, "pirate/coin.wav"),
        mHitSound(pApplication, "pirate/hit.wav"),
        mTextSound(pApplication, "pirate/text.wav"),
        mBackgroundTexture(pApplication, "pirate/background_pirate.png"),
        mCaveTexture(pApplication, "pirate/cave_final.png"),
        mSlotsTexture(pApplication, "pirate/slot_single.png"),
        mSlotMachineTexture(pApplication, "pirate/slot_machine.png"),
        mShipTexture(pApplication, "pirate/ships.png"),
        mCoinTexture(pApplication, "pirate/coin.png"),
        mPlayerTexture(pApplication, "pirate/player.png"),
        mCoinsTexture(pApplication, "pirate/coins.png"),
        mLivesTexture(pApplication, "pirate/live.png"),
        mBoxTexture(pApplication, "pirate/box.png"),
        mStartTextTexture(pApplication, "pirate/start.png"),
        mLevelUpTextTexture(pApplication, "pirate/level_up.png"),
        mGameOverTextTexture(pApplication, "pirate/game_over.png"),

        mBackground(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mCaves(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mSlots(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mSlotMachine(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mShipBrown(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mShipRed(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mCoin(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mPlayer(pApplication, mGraphicsManager, mSoundManager),
        mCoins(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mLives(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mBox(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mStartText(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mLevelUpText(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mGameOverText(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),

        mSpriteBatch(mTimeManager, mGraphicsManager),
        mMoveableBody(pApplication, mInputManager, mPhysicsManager)
{
    Log::info("Creating Pirate");

    mCoinPlaySound = mSoundManager.registerSound(mCoinSound);
    mHitPlaySound = mSoundManager.registerSound(mHitSound);
    mTextPlaySound = mSoundManager.registerSound(mTextSound);

    Sprite *backgroundGraphics = mSpriteBatch.registerSprite(mBackgroundTexture, 640, 384);
    mBackground.registerBackground(backgroundGraphics->location, 640, 384);

    for (int32_t i = 0; i < 3; ++i)
    {
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
}

void Pirate::run()
{
    mEventLoop.run();
}

status Pirate::onActivate()
{
    Log::info("Activating Pirate");
    // Starts managers.
    if (mGraphicsManager.start() != STATUS_OK) return STATUS_KO;
    if (mSoundManager.start() != STATUS_OK) return STATUS_KO;
    mInputManager.start();

    // Plays music and a sound at startup.
    mSoundManager.playBGM(mBGM);

    // Initializes game objects.
    mBackground.initialize();
    mCaves.initialize();
    mShipBrown.initialize(true);
    mShipRed.initialize(false);
    mCoin.initialize();
    //mSlots.initialize();
    //mSlotMachine.initialize();
    mBox.initialize();

    mPlayer.initialize();
    mMoveableBody.initialize();

    mCoins.initialize();
    mLives.initialize();

    mStartText.initialize();
    mLevelUpText.initialize();
    mGameOverText.initialize();

    mGraphicsManager.setSpriteEnabled(mGameController.TEXT_LEVEL_UP_ID, false);
    mGraphicsManager.setSpriteEnabled(mGameController.TEXT_GAME_OVER_ID, false);
    mGraphicsManager.setSpriteEnabled(mGameController.COIN_ANIM_ID, false);

    mTimeManager.reset();
    mGameController.resetStartTime = mTimeManager.now();
    return STATUS_OK;
}

void Pirate::onDeactivate()
{
    Log::info("Deactivating Pirate");
    mGraphicsManager.stop();
    mSoundManager.stop();
}

status Pirate::onStep()
{
    switch (mGameController.state)
    {
        case START:
            return onStartGameStep();
        case TUTORIAL:
            return onTutorialStep();
        case GAME:
            return onGameStep();
        case LEVEL_UP:
            return onLevelUpStep();
        case GAME_OVER:
            return onGameOverStep();
        default:
            return onStartGameStep();
    }
}

void Pirate::onShipCollision()
{
    if (mTimeManager.now() - mGameController.resetStartTime > 3)
    {
        mSoundManager.playSound(mHitPlaySound);
        mPlayer.setInvincible(true);
        mGameController.resetStartTime = mTimeManager.now();
        mGameController.lives--;

        for (int i = 3; i > 0; --i)
        {
            mGraphicsManager.setSpriteEnabled(mGameController.START_LIVES_ID + (i - 1),
                                              mGameController.lives >= i);
        }

        if (mGameController.lives <= 0)
        {
            mGameController.state = GAME_OVER;
            mGraphicsManager.setSpriteEnabled(mGameController.TEXT_GAME_OVER_ID, true);
        }

        mMoveableBody.initialize();
    }
}

void Pirate::onCaveCollision()
{
    for (int i = 0; i < 3; ++i)
    {
        if(mMoveableBody.checkCollisionWithBody(mCaves.getBody(i)))
        {
            if (!mCaves.getCaveFlashing(i))
            {
                switch (i)
                {
                    case 0:
                        mGraphicsManager.setSpriteAnimation(mGameController.CAVE_TOP_LEFT_ID,
                                                            0, 4, 8.0f, true);
                        break;
                    case 1:
                        mGraphicsManager.setSpriteAnimation(mGameController.CAVE_TOP_RIGHT_ID,
                                                            0, 4, 8.0f, true);
                        break;
                    case 2:
                        mGraphicsManager.setSpriteAnimation(mGameController.CAVE_BOTTOM_RIGHT_ID,
                                                            0, 4, 8.0f, true);
                        break;
                    default:
                        break;
                }
                mCaves.setCaveFlashing(i, true);
                mCoin.setLocation(*mMoveableBody.getBody()->location);
                mGraphicsManager.setSpriteAnimation(mGameController.COIN_ANIM_ID ,
                                                    0, 4, 4.0f, true);
                mGraphicsManager.setSpriteEnabled(mGameController.COIN_ANIM_ID, true);
                mSoundManager.playSound(mCoinPlaySound);
            }

            if (!mPlayer.getCarryACoin())
            {
                mPlayer.setCarryACoin(true);
                mPlayer.setCoinsCollected(i);
            }
        }
        else if (mCaves.getCaveFlashing(i))
        {
            switch (i)
            {
                case 0:
                    mGraphicsManager.setSpriteAnimation(mGameController.CAVE_TOP_LEFT_ID,
                                                        0, 1, 0.0f, false);
                    break;
                case 1:
                    mGraphicsManager.setSpriteAnimation(mGameController.CAVE_TOP_RIGHT_ID,
                                                        0, 1, 0.0f, false);
                    break;
                case 2:
                    mGraphicsManager.setSpriteAnimation(mGameController.CAVE_BOTTOM_RIGHT_ID,
                                                        0, 1, 0.0f, false);
                    break;
                default:
                    break;
            }
            mCaves.setCaveFlashing(i, false);
            mGraphicsManager.setSpriteAnimation(mGameController.COIN_ANIM_ID , 0, 1, 0.0f, false);
            mGraphicsManager.setSpriteEnabled(mGameController.COIN_ANIM_ID, false);

        }
    }
}

void Pirate::onStartPointCollision()
{
    mPlayer.setCarryACoin(false);
    int numCoinCollected = 0;

    for (int i = 0; i < 3; ++i)
    {
        if(mPlayer.getCoinsCollected(i))
        {
            numCoinCollected++;
        }
    }

    if (numCoinCollected > 0)
    {
        mGraphicsManager.setSpriteAnimation(mGameController.BOX_ID, 1, 1, 0.0f, false);
    }

    mGameController.gold = numCoinCollected;

    for (int i = 3; i > 0; --i)
    {
        mGraphicsManager.setSpriteEnabled(mGameController.START_COIN_ID + (i - 1),
                                          mGameController.gold >= i);
    }
    
    if (mGameController.gold == 3) mGameController.state = LEVEL_UP;
}

status Pirate::onGameStep()
{
    mTimeManager.update();
    mPhysicsManager.update();
    mShipBrown.update();
    mShipRed.update();
    mCoin.update();

    //mSlots.update();
    // Updates modules.
    mMoveableBody.update();
    if (mMoveableBody.checkCollisionWithBody(mShipBrown.getBody()))
    {
        onShipCollision();
    }

    if (mMoveableBody.checkCollisionWithBody(mShipRed.getBody()))
    {
        onShipCollision();
    }

    onCaveCollision();

    if (mMoveableBody.checkCollisionWithBody(mBox.getBody()))
    {
        onStartPointCollision();
    }

    // TODO: candidate for separate function
    if (mPlayer.getInvincible())
    {
        if (mTimeManager.now() - mGameController.resetStartTime > 3)
        {
            mGraphicsManager.setSpriteEnabled(mGameController.PLAYER_ID, true);
            mPlayer.setInvincible(false);
        }
        else
        {
            static double blinkTime = mTimeManager.now();

            if (mTimeManager.now() - blinkTime > 0.2)
            {
                mGraphicsManager.setSpriteEnabled(mGameController.PLAYER_ID, !mGraphicsManager.getSpriteEnabled(mGameController.PLAYER_ID));
                blinkTime = mTimeManager.now();
            }
        }
    }

    return mGraphicsManager.update();
}

status Pirate::onStartGameStep()
{
    if (mTimeManager.now() - mGameController.resetStartTime > 1)
    {
        if (mInputManager.getTouched())
        {
            mGameController.state = GAME;
            mGraphicsManager.setSpriteEnabled(mGameController.TEXT_START_ID, false);
        }
    }

    mTimeManager.update();
    mPhysicsManager.update();

    return mGraphicsManager.update();
}

status Pirate::onTutorialStep()
{
    mTimeManager.update();
    mPhysicsManager.update();

    return mGraphicsManager.update();
}

status Pirate::onGameOverStep()
{
    if (mTimeManager.now() - mGameController.resetStartTime > 3)
    {
        if (mInputManager.getTouched())
        {
            mGameController.state = START;
            mGraphicsManager.setSpriteEnabled(mGameController.TEXT_START_ID, true);
            mGraphicsManager.setSpriteEnabled(mGameController.TEXT_GAME_OVER_ID, false);
            mGameController.gold = 0;
            mGameController.lives = 3;
            mPlayer.clearAllCollectedCoins();
            mPlayer.setCarryACoin(false);
            mShipBrown.setShipSpeed(mGameController.PIRATE_SHIP_SPEED);
            mShipRed.setShipSpeed(mGameController.PIRATE_SHIP_SPEED);
            mGraphicsManager.setSpriteAnimation(mGameController.BOX_ID, 0, 1, 0.0f, false);

            for (int i = 3; i > 0; --i)
            {
                mGraphicsManager.setSpriteEnabled(mGameController.START_LIVES_ID + (i - 1),
                                                  mGameController.lives >= i);
            }

            mGameController.resetStartTime = mTimeManager.now();
        }
    }

    mTimeManager.update();
    mPhysicsManager.update();

    return mGraphicsManager.update();
}

status Pirate::onLevelUpStep()
{
    static bool once = true;

    if (once)
    {
        mSoundManager.playSound(mTextPlaySound);
        mGraphicsManager.setSpriteEnabled(mGameController.TEXT_LEVEL_UP_ID, true);
        mGameController.gold = 0;
        mGraphicsManager.setSpriteAnimation(mGameController.BOX_ID, 0, 1, 0.0f, false);

        for (int i = 3; i > 0; --i)
        {
            mGraphicsManager.setSpriteEnabled(mGameController.START_COIN_ID + (i - 1),
                                              mGameController.gold >= i);
        }

        mPlayer.clearAllCollectedCoins();
        mMoveableBody.initialize();

        once = false;
    }

    if (mInputManager.getTouched())
    {
        mShipBrown.setShipSpeed(mShipBrown.getShipSpeed() + 0.25f * mShipBrown.getShipSpeed());
        mShipRed.setShipSpeed(mShipRed.getShipSpeed() + 0.25f * mShipRed.getShipSpeed());
        mGameController.state = GAME;
        mGraphicsManager.setSpriteEnabled(mGameController.TEXT_LEVEL_UP_ID, false);
        once = true;
    }

    mTimeManager.update();
    mPhysicsManager.update();

    return mGraphicsManager.update();
}

void Pirate::onStart()
{
    Log::info("onStart");
}

void Pirate::onResume()
{
    Log::info("onResume");
}

void Pirate::onPause()
{
    Log::info("onPause");
}

void Pirate::onStop()
{
    Log::info("onStop");
}

void Pirate::onDestroy()
{
    Log::info("onDestroy");
}

void Pirate::onSaveInstanceState(void** pData, size_t* pSize)
{
    Log::info("onSaveInstanceState");
}

void Pirate::onConfigurationChanged()
{
    Log::info("onConfigurationChanged");
}

void Pirate::onLowMemory()
{
    Log::info("onLowMemory");
}

void Pirate::onCreateWindow()
{
    Log::info("onCreateWindow");
}

void Pirate::onDestroyWindow()
{
    Log::info("onDestroyWindow");
}

void Pirate::onGainFocus()
{
    Log::info("onGainFocus");
}

void Pirate::onLostFocus()
{
    Log::info("onLostFocus");
}

