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

bool GuiObjView::Load(File* f)
{
  // Load name, pos, size, obj filename
  if (!GuiElement::Load(f))
  {
    return false; 
  }
  // TODO child loads itself
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

  if (mesh)
  {
    SetObjMesh(mesh);
  }
  else
  {
    f->ReportError("Failed to load obj mesh " + objFile);
    // OK, just display nothing
    //return false;
  }

  // TODO Calc AABB for camera

  return true;
}


GuiObj::GuiObj()
{
  xrot = 0;
  yrot = 0;
  Ldrag = false;
  Mdrag = false;
  Rdrag = false;
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

    const float FOVY = 60.0f;  // TODO
    const float NEAR_PLANE = 1.0f;
    const float FAR_PLANE = 4000.0f;
    float aspect = 1.3f;
    AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::PushMatrix();
    AmjuGL::SetIdentity();

    AmjuGL::LookAt(0, 100, 300,  0, 0, 0,  0, 1, 0); // TODO
    AmjuGL::Translate(pos.x, pos.y, pos.z);
    AmjuGL::RotateX(xrot);
    AmjuGL::RotateY(yrot);

    // TODO Lighting, texturing, wireframe flags

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

bool GuiObj::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    Ldrag = mbe.isDown;
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_MIDDLE)
  {
    Mdrag = mbe.isDown;
  }
  else if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    Rdrag = mbe.isDown;
  }

  return false; 
}

bool GuiObj::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;
  float dx = ce.x - oldx;
  float dy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  if (Ldrag)
  {
    static const float SENSITIVITY = 100.0f;

    xrot += dy * SENSITIVITY;
    yrot += dx * SENSITIVITY;
  }
  else if (Mdrag)
  {
    static const float SENSITIVITY = 100.0f;

    pos.y += dy * SENSITIVITY;
    pos.x += dx * SENSITIVITY;
  }
  else if (Rdrag)
  {
    static const float SENSITIVITY = 100.0f;

    pos.z += dy * SENSITIVITY;
  }
  return false; 
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

