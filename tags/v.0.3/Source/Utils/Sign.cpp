#include "Sign.h"

namespace Amju
{
float Sign(float f)
{
  if (f < 0)
  {
    return -1.0f;
  }
  else if (f > 0)
  {
    return 1.0f;
  }
  return 0;
}
}
