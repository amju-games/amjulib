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

  bool Load(File*) override;
  void SetObjMesh(ObjMesh*);
};

class GuiObj : public GuiElement
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  GuiObj();

  void Draw() override;
  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;

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

