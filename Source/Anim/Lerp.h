#ifndef AMJU_LERP_H_INCLUDED
#define AMJU_LERP_H_INCLUDED

// Linear Interpolation for any type which supports 
// operator+, operator- and operator*

#include "AmjuAssert.h"

namespace Amju
{
template <class T>
T Lerp(const T& t1, const T& t2, float between)
{
  Assert(between >= 0);
  Assert(between <= 1.0f);

  return t1 + (t2 - t1) * between;
}

}

#endif

