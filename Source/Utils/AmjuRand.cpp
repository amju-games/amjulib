#include <AmjuFirst.h>
#include <stdlib.h>
#include "AmjuRand.h"
#include <AmjuFinal.h>

namespace Amju
{
void Randomise()
{
  time_t tt;
  srand(time(&tt));
}

float Rnd(float min, float max)
{
  return (float)rand() / (float)RAND_MAX * (max - min) + min;
}
}

