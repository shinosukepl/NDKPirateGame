#include "Player.h"

static const float INITIAL_X = 0.5f;
static const float INITIAL_Y = 0.25f;

Player::Player(android_app* pApplication, GraphicsManager& pGraphicsManager,
               SoundManager& pSoundManager) :
        mGraphicsManager(pGraphicsManager),
        mGraphics(NULL),
        mSoundManager(pSoundManager),
        mCollisionSound(NULL),
        mIsCarryACoin(false),
        mIsInvincible(false),
        mCoinsCollected()
{

}

void Player::registerPlayer(Sprite* pGraphics, Sound* pCollisionSound)
{
    mGraphics = pGraphics;
    mCollisionSound = pCollisionSound;
}

void Player::initialize()
{
    mGraphics->location.x = INITIAL_X * mGraphicsManager.getRenderWidth();
    mGraphics->location.y = INITIAL_Y * mGraphicsManager.getRenderHeight();
    for (int i = 0; i < 3; ++i)
    {
        mCoinsCollected[i] = false;
    }
    mIsCarryACoin = false;

    //mSoundManager.playSound(mCollisionSound);
}

void Player::clearAllCollectedCoins()
{
    for (int i = 0; i < 3; ++i)
    {
        mCoinsCollected[i] = false;
    }
}
