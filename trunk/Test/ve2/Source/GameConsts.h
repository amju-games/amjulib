#ifndef GAME_CONSTS_H_INCLUDED
#define GAME_CONSTS_H_INCLUDED

#include <Colour.h>

namespace Amju
{
extern Colour LURK_FG;
extern Colour LURK_BG;

extern const char* LAST_MSG_SENT_KEY;

// Simplify: Just score.
// In single-player mode, score goes up when we eat a food.
// In multi-player, we get a point for giving food to someone, AND a point when we are given food
extern const char* SCORE_KEY;

/*
extern const char* FOOD_KEY; // ?
*/

extern const char* FOOD_EATEN_KEY;
extern const char* FOOD_RECEIVED_KEY;
extern const char* FOOD_GIVEN_KEY;
extern const char* HEALTH_KEY;

extern const int FOOD_START_ID;
extern const int BADDDIE_START_ID;
extern const int BULLET_START_ID;
}

#endif
