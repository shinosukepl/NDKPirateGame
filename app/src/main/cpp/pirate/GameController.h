#pragma once

#include <cstdint>

enum GameState
{
    START,
    TUTORIAL,
    GAME,
    LEVEL_UP,
    GAME_OVER
};

struct GameController {
    GameController():
            state(START),
            lives(3),
            gold(0),
            level(1),
            resetStartTime(0.0f)
    {
    }

    const static int PLAYER_ID = 1;
    const static int COIN_ANIM_ID = 2;
    const static int PIRATE_SHIP_BROWN_ID = 5;
    const static int PIRATE_SHIP_RED_ID = 6;
    const static int CAVE_TOP_LEFT_ID = 10;
    const static int CAVE_TOP_RIGHT_ID = 11;
    const static int CAVE_BOTTOM_RIGHT_ID = 12;
    const static int START_COIN_ID = 20;
    const static int START_LIVES_ID = 25;
    const static int BOX_ID = 30;
    const static int TEXT_START_ID = 100;
    const static int TEXT_LEVEL_UP_ID = 101;
    const static int TEXT_GAME_OVER_ID = 102;
    const float PIRATE_SHIP_SPEED = 40.0f;

    GameState state;
    int32_t lives;
    int32_t gold;
    int32_t level;
    double resetStartTime;
};
