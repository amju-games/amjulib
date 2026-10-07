// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
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
