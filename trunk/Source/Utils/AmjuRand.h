#ifndef AMJU_RAND_H_INCLUDED
#define AMJU_RAND_H_INCLUDED

namespace Amju
{
// Return rand float between min and max, min <= r < max
// Uses std lib rand()
float Rnd(float min, float max);
}

#endif
