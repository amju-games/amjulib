#include <AmjuFirst.h>
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
Colour LURK_FG(1, 1, 1, 1);
Colour LURK_BG(0, 0, 0, 1);

const char* LAST_MSG_SENT_KEY = "last_msg_sent";

// Simplify: Just score.
// In single-player mode, score goes up when we eat a food.
// In multi-player, we get a point for giving food to someone, AND a point when we are given food

const char* SCORE_KEY = "score";

/*
const char* FOOD_KEY = "Foods";
*/
const char* FOOD_EATEN_KEY = "food-eaten";
const char* FOOD_RECEIVED_KEY = "food-recv";
const char* FOOD_GIVEN_KEY = "food-given";

// IDs for local game objects
const int FOOD_START_ID =    200000;
const int BADDDIE_START_ID = 300000;
const int BULLET_START_ID =  400000;

}
