#ifndef GS_FINISH_H
#define GS_FINISH_H

#include <GameState.h>
#include <GuiElement.h>

namespace Amju
{
class GSFinish : public GameState
{
public:
  static const char* NAME;

  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

protected:
  PGuiElement m_gui;
};
}

#endif

