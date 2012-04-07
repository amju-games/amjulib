#ifndef GS_EDIT_H_INCLUDED
#define GS_EDIT_H_INCLUDED

#include <set>
#include <string>
#include <vector>
#include <GuiElement.h>
#include <GuiMenu.h>
#include "GSBase.h"
#include "Ve1Object.h"

namespace Amju
{
// Location type, representing one Location on server
struct Loc
{
  Loc(int id, const std::string& name) : m_id(id), m_name(name) { } 

  int m_id;
  std::string m_name;
};
typedef std::vector<Loc> Locs;


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

  // Called when server responds with new object ID
  void OnNewObjectId(int id);
  void ShowPropsForObj(int id);

  // Set list of locations - called when we get response from server
  void SetLocs(const Locs& locs);
  void ChooseLocation();
  void ShowLocationList(bool);
  void GoToSelectedLocation();

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
  bool m_isPicking;
  int m_newObjectId;
  // Vectors for dragging
  Vec3f m_right;
  Vec3f m_up;
  bool m_getDragVec; // if true get new values for m_up and m_right

  // All locations, so we can jump from one to another
  Locs m_locs;
};

typedef Singleton<GSEdit> TheGSEdit;
}

#endif

