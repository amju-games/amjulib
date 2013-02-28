#ifndef GS_MAIN_H_INCLUDED
#define GS_MAIN_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"
#include <Vec2.h>
#include <GuiElement.h>
#include <GuiMenu.h>
#include "MsgManager.h"

namespace Amju 
{
class GameObject;

class GSMainListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};

class GSMain : public GSBase
{
protected:
  GSMain();
  friend class Singleton<GSMain>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
  virtual void OnDeactive();

  bool OnCursorEvent(const CursorEvent&);
  bool OnMouseButtonEvent(const MouseButtonEvent&);

  void SetNumPlayersOnline(int);

  // So chat console reduces the width of the 3D scene
  void SetViewWidth(float w);

  void OnMenuClickedAway();
  void OnQuitButton();

private:
  void DoMoveRequest();
  bool ShowObjectMenu(GameObject*);

private:
  Vec2f m_mouseScreen;
  bool m_moveRequest;
  PGuiElement m_gui;
  RCPtr<GuiMenu> m_menu;
  int m_numPlayersOnline;

  // Debug camera
  float m_yRot;

  RCPtr<EventListener> m_listener;

  // Adjust viewport width so chat fits down side..?
  float m_viewportWidth;

  bool m_quitConfirm;
};
typedef Singleton<GSMain> TheGSMain;
} // namespace
#endif