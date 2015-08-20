#pragma once

#include "GSTemple.h"
#include <Singleton.h>

namespace Amju
{
class GSWaterTemple : public GSTemple
{
  GSWaterTemple();
  friend class Singleton<GSWaterTemple>;

public:
};

typedef Singleton<GSWaterTemple> TheGSWaterTemple;
}

