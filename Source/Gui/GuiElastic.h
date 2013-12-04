#pragma once

#include "GuiComposite.h"

namespace Amju
{
// Decorator: moves child from an offset to the final intended position 
class GuiElastic : public GuiComposite
{
public:
  static const char* NAME;
  GuiElastic();
  virtual bool Load(File*);
  virtual void Draw(); // update offset

protected:
  Vec2f m_acc;
  Vec2f m_vel;
  Vec2f m_origVel;
  int m_bounceX, m_bounceY;
};
}

