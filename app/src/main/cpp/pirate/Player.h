#pragma once

#include "engine/GraphicsManager.hpp"
#include "engine/Sprite.hpp"
#include "engine/SoundManager.hpp"
#include "engine/Sound.hpp"
#include "PirateShip.h"

class Player
{
public:
    Player(android_app* pApplication, GraphicsManager& pGraphicsManager,
           SoundManager& pSoundManager);

    void registerPlayer(Sprite* pGraphics, Sound* pCollisionSound);
    void initialize();
    void clearAllCollectedCoins();

    inline bool getCarryACoin() const {return mIsCarryACoin;};
    inline bool getCoinsCollected(int caveNum) const {return mCoinsCollected[caveNum];};
    inline bool getInvincible() const {return mIsInvincible;};

    inline void setCarryACoin(bool isCarryACoin) {mIsCarryACoin = isCarryACoin;};
    inline void setCoinsCollected(int caveNum) {mCoinsCollected[caveNum] = true;};
    inline void setInvincible(bool isInvincible) {mIsInvincible = isInvincible;};



private:
    Player(const Player&);
    void operator=(const Player&);

    GraphicsManager& mGraphicsManager;
    SoundManager& mSoundManager;
    Sprite* mGraphics;
    Sound* mCollisionSound;
    bool mIsInvincible;
    bool mIsCarryACoin;
    bool mCoinsCollected[3];


};
