#pragma once

#include <AmjuGL.h>
#include <DrawDecorator.h>
#include <FullScreenPostProcess.h>

namespace Amju
{
class BarrelDist : public DrawDecorator, public FullScreenPostProcess
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

