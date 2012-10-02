#ifndef GUI_OBJ_VIEW_H_INCLUDED
#define GUI_OBJ_VIEW_H_INCLUDED

#include <ObjMesh.h>
#include "GuiWindow.h"

namespace Amju
{
class GuiObjView : public GuiWindow
{
public:
  static const char* NAME;

  virtual void Draw();
  virtual bool Load(File*);

  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
  virtual bool OnCursorEvent(const CursorEvent&);

  void SetObjMesh(ObjMesh*);

private:
  // TODO Should be a scene graph, right ?
  PObjMesh m_mesh;
};

GuiElement* CreateObjView();
}

#endif

