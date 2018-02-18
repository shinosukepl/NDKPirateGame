#pragma once

#include <cstdlib>

typedef int32_t status;

const status STATUS_OK   = 0;
const status STATUS_KO   = -1;
const status STATUS_EXIT = -2;

struct Location
{
    Location(): x(0.0f), y(0.0f) {};

    float x; float y;
};

// Generates a random value in the range [O, pMax].
#define RAND(pMax) (float(pMax) * float(rand()) / float(RAND_MAX))
