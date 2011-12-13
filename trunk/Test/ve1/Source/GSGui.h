#ifndef GS_GUI_H_INCLUDED
#define GS_GUI_H_INCLUDED

#include "GSBase.h"
#include <GuiElement.h>

namespace Amju 
{
class GSGui : public GSBase
{
protected:
  GSGui();
public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

protected:
  PGuiElement m_gui;
};
} // namespace
#endif
