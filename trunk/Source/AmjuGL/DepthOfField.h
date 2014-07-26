#pragma once

#include <AmjuGL.h>
#include <DrawDecorator.h>
#include <FullScreenPostProcess.h>

namespace Amju
{
// Create one of these directly, not through AmjuGL::Create
class DepthOfField : public DrawDecorator, public FullScreenPostProcess
{
public:
  virtual bool Init();
  virtual void Draw();
};
}

