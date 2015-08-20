#pragma once

#include <Singleton.h>
#include "GSTeapotBase.h"

namespace Amju 
{
class GSEnvMap : public GSTeapotBase
{
  GSEnvMap();
  friend class Singleton<GSEnvMap>;

public:
  virtual void DrawScene();
};
typedef Singleton<GSEnvMap> TheGSEnvMap;
} // namespace

