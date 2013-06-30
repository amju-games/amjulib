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
class Furniture;

// Need this because EventPoller expects all listeners to be ref counted/on heap.
// GameStates are singletons, not ref counted.
class GSMainListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnKeyEvent(const KeyEvent&);
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

  // Context-sensitive menu for the currently selected object.
  // This is now a row of buttons rather than a GuiMenu. This is to improve usability. The menu
  //  got in the way of playing the game when displayed over the selected object.
  GuiComposite* GetContextMenu();
  void ClearContextMenu();
  void AddMenuItem(const std::string& text, GuiCommand* command);

  // Show button to drop carried item
  //void ShowDropButton(Furniture*, bool show); 

  void SetScore(int);
  void SetHealth(int);
  void ResetHud();

private:
  void DoMoveRequest();
  bool ShowObjectMenu(GameObject*);

private:
  Vec2f m_mouseScreen;
  bool m_moveRequest;
  PGuiElement m_gui;

  RCPtr<GuiMenu> m_menu;
  // the last object which displayed a menu
  GameObject* m_menuObject;
  // If true, the last object menu was dismissed with no choice selected
  bool m_wasClickedAway;

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
