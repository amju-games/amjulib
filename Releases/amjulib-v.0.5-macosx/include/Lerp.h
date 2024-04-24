// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

namespace Amju
{
// * Lerp *
// Linear interpolation template function
template <typename T>
T Lerp(const T& t0, const T& t1, float f)
{
  return t0 + f * (t1 - t0);
}
}
