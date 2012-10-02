#include <StringUtils.h>
#include "GuiObjView.h"

namespace Amju
{
const char* GuiObjView::NAME = "gui-obj-view";

GuiElement* CreateObjView()
{
  GuiObjView* gov = new GuiObjView;
  GuiObj* go = new GuiObj;
  gov->AddChild(go);
  return gov;
}

void GuiObj::Draw()
{
  if (m_mesh)
  {
    // TODO Controllable camera
    Assert(AmjuGL::GetMatrixMode() == AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::PushMatrix();
    AmjuGL::SetIdentity();
    AmjuGL::SetPerspectiveProjection(45.0f, 1.0, 1.0, 1000.0f); // TODO load in ? Settable ?
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::PushMatrix();
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(0, 100, 300,  0, 0, 0,  0, 1, 0);

    static float degs = 0;
    degs += 0.1f;
    AmjuGL::RotateY(degs);
    AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ | AmjuGL::AMJU_DEPTH_WRITE);
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
    m_mesh->Draw();    
    AmjuGL::PopAttrib();

    AmjuGL::PopMatrix();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::PopMatrix();
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
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

  PObjMesh mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(objFileNoPath);

  // After loading, revert to original file root
  File::SetRoot(origRoot, "/");

  if (!mesh)
  {
    f->ReportError("Failed to load obj mesh " + objFile);
    return false;
  }
  SetObjMesh(mesh);

  // TODO Calc AABB for camera

  return true;
}

bool GuiObj::OnMouseButtonEvent(const MouseButtonEvent&)
{
  return false; // TODO
}

bool GuiObj::OnCursorEvent(const CursorEvent&)
{
  return false; // TODO
}

void GuiObjView::SetObjMesh(ObjMesh* mesh)
{
  Assert(m_children.size() == 1);
  GuiObj* go = dynamic_cast<GuiObj*>(m_children[0].GetPtr());
  Assert(go);
  go->SetObjMesh(mesh);
}

void GuiObj::SetObjMesh(ObjMesh* mesh)
{
  m_mesh = mesh;
}

} 

