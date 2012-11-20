#include <stdlib.h>
#include "AmjuRand.h"

namespace Amju
{
float Rnd(float min, float max)
{
  return (float)rand() / (float)RAND_MAX * (max - min) + min;
}
}

