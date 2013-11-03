#ifndef GS_VIEWOBJGUI_H_INCLUDED
#define GS_VIEWOBJGUI_H_INCLUDED

#include <Singleton.h>
#include <GameState.h>
#include <GuiElement.h>

namespace Amju 
{
class GSViewObjGui : public GameState
{
  GSViewObjGui();
  friend class Singleton<GSViewObjGui>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:
  RCPtr<GuiElement> m_gui;
};
typedef Singleton<GSViewObjGui> TheGSViewObjGui;
} // namespace
#endif
