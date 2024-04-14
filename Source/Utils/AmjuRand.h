#ifndef AMJU_RAND_H_INCLUDED
#define AMJU_RAND_H_INCLUDED

#include <algorithm>

namespace Amju
{
// Set random seed to current time
void Randomise();

// Return rand float between min and max, min <= r < max
// Uses std lib rand()
float Rnd(float min, float max);

// For random shuffle, returns values >= 0 and < n
int RandomInt(int n);

// Cross-platform Random Shuffle
// From http://en.cppreference.com/w/cpp/algorithm/random_shuffle
// RandomFunc r is a functor returning a random int between 0 and n-1 inclusive.
// i.e. [0, n)
// RandomInt above is a good choice!
template<class RandomIt, class RandomFunc>
void RandomShuffle(RandomIt first, RandomIt last, RandomFunc r)
{
  typename std::iterator_traits<RandomIt>::difference_type i, n;
  n = last - first;
  for (i = n - 1; i > 0; --i) {
    using std::swap;
    swap(first[i], first[r(i + 1)]);
  }
}

}

#endif
