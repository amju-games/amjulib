#ifndef AMJU_RAND_H_INCLUDED
#define AMJU_RAND_H_INCLUDED

namespace Amju
{
// Set random seed to current time
void Randomise();

// Return rand float between min and max, min <= r < max
// Uses std lib rand()
float Rnd(float min, float max);
}

#endif
