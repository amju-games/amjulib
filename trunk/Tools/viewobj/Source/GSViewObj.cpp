#include "GSViewObj.h"
#include <AmjuGL.h>
#include <ObjMesh.h>
#include <iostream>
#include <Timer.h>

namespace Amju
{
static ObjMesh* mesh = 0;

static float xrot = 0;
static float yrot = 0;
static bool drag = false;
static Vec3f pos;
static Vec3f vel;

GSViewObj::GSViewObj()
{
  //m_nextState=...
}

void GSViewObj::Update()
{
  pos += vel * TheTimer::Instance()->GetDt();
}

void GSViewObj::Draw()
{
  //AmjuGL::SetIsWireFrameMode(true);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_PLANE = 1.0f;
  const float FAR_PLANE = 4000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(FOVY, aspect, NEAR_PLANE, FAR_PLANE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::Translate(pos.x, pos.y, pos.z);
  AmjuGL::RotateX(xrot);
  AmjuGL::RotateY(yrot);

  AmjuGL::LookAt(0, 0, 0,   0, 100, 2000,  0, 1, 0);

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  mesh->Draw();
}

void GSViewObj::Draw2d()
{
}

void GSViewObj::OnActive()
{
  AmjuGL::SetClearColour(Colour(0, 1, 0, 1));
  mesh = new ObjMesh;
  if (mesh->Load("model.obj"))
  {
    std::cout << "Successfully loaded obj file!\n";
  }
  else
  {
    std::cout << "FAILED TO LOAD MODEL!!\n";
  }
}

bool GSViewObj::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;
  float diffx = ce.x - oldx;
  float diffy = ce.y - oldy;
  oldx = ce.x;
  oldy = ce.y;

  if (drag)
  {
    static const float SENSITIVITY = 100.0f;

    xrot += diffy * SENSITIVITY;
    yrot += diffx * SENSITIVITY;
  }

  return false;
}

bool GSViewObj::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  drag = mbe.isDown;
  return false;
}

bool GSViewObj::OnKeyEvent(const KeyEvent& ke)
{
  static const float SPEED = 50.0f;

  if (ke.keyDown)
  {
    switch (ke.keyType)
    {
    case AMJU_KEY_UP:
      vel.z = SPEED;
      break;
    
    case AMJU_KEY_DOWN:
      vel.z = -SPEED;
      break;

    case AMJU_KEY_LEFT:
      vel.x = SPEED;
      break;

    case AMJU_KEY_RIGHT:
      vel.x = -SPEED;
      break;
    } 
  }
  else
  {
    switch (ke.keyType)
    {
    case AMJU_KEY_DOWN:
    case AMJU_KEY_UP:
      vel.z = 0;
      break;

    case AMJU_KEY_LEFT:
    case AMJU_KEY_RIGHT:
      vel.x = 0;
      break;
    }
  }

  return true;
}

} // namespace
