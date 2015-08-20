#pragma once

#include <Singleton.h>
#include "GSTeapotBase.h"

namespace Amju 
{
class GSNormalMap : public GSTeapotBase
{
  GSNormalMap();
  friend class Singleton<GSNormalMap>;

public:
  virtual void DrawScene();
};
typedef Singleton<GSNormalMap> TheGSNormalMap;
} // namespace

