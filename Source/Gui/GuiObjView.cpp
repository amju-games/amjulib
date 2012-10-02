#include <StringUtils.h>
#include "GuiObjView.h"

namespace Amju
{
const char* GuiObjView::NAME = "gui-obj-view";

GuiElement* CreateObjView()
{
  GuiWindow* w = new GuiWindow;
  GuiObjView* gov = new GuiObjView;
  w->AddChild(gov);
  return w;
}

void GuiObjView::Draw()
{
  if (m_mesh)
  {
    // TODO Controllable camera

    m_mesh->Draw();    
  }
}

bool GuiObjView::Load(File* f)
{
  // Load name, pos, size, obj filename
  if (!GuiElement::Load(f))
  {
    return false;
  }
  std::string objFile;
  if (!f->GetDataLine(&objFile))
  {
    f->ReportError("Expected obj mesh file name");
    return false;
  }

  // Set root so we can load mtl file and textures in same dir as obj file
  std::string path = GetFilePath(objFile);
  std::string origRoot = File::GetRoot();
  File::SetRoot(path , "/"); // TODO change SetRoot params

  std::string objFileNoPath = StripPath(objFile);

  //bool binary = (GetFileExt(objFile) == "objb");
  //RCPtr<ObjMesh> mesh = new ObjMesh;
  //bool loaded = mesh->Load(objFile, binary);

  m_mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(objFileNoPath);

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  if (!m_mesh)
  {
    f->ReportError("Failed to load obj mesh " + objFile);
    return false;
  }
  // TODO Calc AABB for camera

  return true;
}

bool GuiObjView::OnMouseButtonEvent(const MouseButtonEvent&)
{
  return false; // TODO
}

bool GuiObjView::OnCursorEvent(const CursorEvent&)
{
  return false; // TODO
}

void GuiObjView::SetObjMesh(ObjMesh* mesh)
{
  m_mesh = mesh;
}

} 

