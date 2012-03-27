#ifndef GS_EDIT_H_INCLUDED
#define GS_EDIT_H_INCLUDED

#include <set>
#include <GuiElement.h>
#include <GuiMenu.h>
#include "GSBase.h"
#include "Ve1Object.h"

namespace Amju
{
class GSEdit : public GSBase
{
private:
  GSEdit();
  friend class Singleton<GSEdit>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);

private:
  int GetUniqueId();
  void OnNextLocation();
  void CreateContextMenu();

private:
  typedef std::set<Ve1Object*> SelObjects;
  SelObjects m_selectedObjects;
  PGuiElement m_gui;
  RCPtr<GuiMenu> m_menu;
  bool m_mouseDownLeft;
  bool m_mouseDownRight;
  Vec2f m_mouseScreen;
  int m_location; 
};

typedef Singleton<GSEdit> TheGSEdit;
}

#endif

