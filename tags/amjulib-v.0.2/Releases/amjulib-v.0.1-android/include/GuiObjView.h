#ifndef GUI_OBJ_VIEW_H_INCLUDED
#define GUI_OBJ_VIEW_H_INCLUDED

#include <ObjMesh.h>
#include "GuiWindow.h"

namespace Amju
{
// Same idea as a GuiListBox, GuiObjView is a window, hiding the 
//  inner GUI element which draws the OBJ mesh.
class GuiObj;

class GuiObjView : public GuiWindow
{
public:
  static const char* NAME;

  virtual bool Load(File*);
  void SetObjMesh(ObjMesh*);
};

class GuiObj : public GuiElement
{
public:
  GuiObj();

  virtual void Draw();
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnCursorEvent(const CursorEvent&);
  void SetObjMesh(ObjMesh*);
  
private:
  // TODO Should be a scene graph, right ?
  PObjMesh m_mesh;

  float xrot;
  float yrot;
  bool Ldrag;
  bool Mdrag;
  bool Rdrag;
  Vec3f pos;

};

GuiElement* CreateObjView();
}

#endif

