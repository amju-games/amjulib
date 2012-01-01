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

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);

  bool CanShowMsg() const;
  void ShowMsg(const MsgManager::Msg& msg);
  void OnChatSend();
  void ActivateChat(bool active, int recipId);

private:
  void DoMoveRequest();
  void ShowObjectMenu(GameObject*);

private:
  Vec2f m_mouseScreen;
  bool m_moveRequest;
  PGuiElement m_gui;
  RCPtr<GuiMenu> m_menu;

  // Chat recipient
  int m_lastRecipId;
};
typedef Singleton<GSMain> TheGSMain;
} // namespace
#endif
