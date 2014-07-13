#pragma once

#include <AmjuGL.h>
#include <DrawDecorator.h>

namespace Amju
{
class DepthOfField : public DrawDecorator
{
public:
  enum { DRAWABLE_TYPE_ID = 6 };

  virtual bool Init() { return true; }
  virtual void Draw() { m_drawFunc(); }
};
}

