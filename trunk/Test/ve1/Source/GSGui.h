#ifndef GS_GUI_H_INCLUDED
#define GS_GUI_H_INCLUDED

#include "GSBase.h"
#include <GuiElement.h>
#include <GuiImage.h>
//#include "TextDraw.h"

namespace Amju 
{
class GSGui : public GSBase //, public TextDraw
{
protected:
  GSGui();
public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

protected:
  bool LoadCogTestBg();
  void DrawCogTestBg();
 
protected:
  PGuiElement m_gui;
  GuiImage m_bgImage;
  bool m_showLurk;
};
} // namespace
#endif
