#ifndef GS_GUITEST_H_INCLUDED
#define GS_GUITEST_H_INCLUDED

#include <Singleton.h>
#include <GuiMenu.h>
#include "GSGui.h"

namespace Amju 
{
class GSGuiTest : public GSGui
{
  GSGuiTest();
  friend class Singleton<GSGuiTest>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

protected:
  RCPtr<GuiMenu> m_menu;
};
typedef Singleton<GSGuiTest> TheGSGuiTest;
} // namespace
#endif
