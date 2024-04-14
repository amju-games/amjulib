#include <AmjuFirst.h>
#include <stdlib.h>
#include <time.h>
#include "AmjuRand.h"
#include "MT19937.h"
#include <AmjuFinal.h>

namespace Amju
{
void Randomise()
{
  time_t tt;
  srand((unsigned int)time(&tt));
}

float Rnd(float min, float max)
{
  return (float)rand() / (float)RAND_MAX * (max - min) + min;
}

int RandomInt(int high)
{
  int i = genrand_int32() % high;
#ifdef RAND_DEBUG 
  std::cout << "==RDSRandom: " << i << "\n";
#endif
  return i;
}
}

