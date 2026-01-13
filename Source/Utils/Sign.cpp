#include <AmjuFirst.h>
#include "Sign.h"
#include <AmjuFinal.h>

namespace Amju
{
template <typename T>
T Sign(T t)
{
  if (t < 0)
  {
    return static_cast<T>(-1);
  }
  else if (t > 0)
  {
    return static_cast<T>(1);
  }
  return 0;
}
}
