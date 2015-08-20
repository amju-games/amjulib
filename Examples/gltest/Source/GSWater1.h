#pragma once

#include <Singleton.h>
#include "GSWaterBase.h"

namespace Amju 
{
class GSWater1 : public GSWaterBase
{
  GSWater1();
  friend class Singleton<GSWater1>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();
  virtual void CreateTweakMenu() override;

private:
};
typedef Singleton<GSWater1> TheGSWater1;
} // namespace

