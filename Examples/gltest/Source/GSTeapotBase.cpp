#include "GSTeapotBase.h"
#include <AmjuGL.h>
#include <Teapot.h>
#include <Timer.h>
#include "Tweakable.h"

namespace Amju
{
static const float INITIAL_ROTATE_VEL = 30.0f;

GSTeapotBase::GSTeapotBase()
{
  //m_nextState=...
 
  m_rotateX = false;
  m_rotateY = false;
  m_xRot = 0;
  m_yRot = 0;
  m_xRotVel = 0;
  m_yRotVel = 0;
}

void GSTeapotBase::Update()
{
  GSBase::Update();

  float dt = TheTimer::Instance()->GetDt();
  if (m_rotateX)
  {
    m_xRot += m_xRotVel * dt;
  }
  if (m_rotateY)
  {
    m_yRot += m_yRotVel * dt;
  }
}

void GSTeapotBase::DrawTeapot()
{
  AmjuGL::SetClearColour(Colour(0.2f, 0.2f, 0.2f, 1));
  GSBase::DrawHelp();

  const float size = 2.5f;
  static Teapot tp(size);

  AmjuGL::PushMatrix();

  // Rotate teapot
  AmjuGL::RotateX(m_xRot); 
  AmjuGL::RotateY(m_yRot);

  tp.Draw();
  AmjuGL::PopMatrix();
}

void GSTeapotBase::CreateTweakMenu()
{
  GSBase::CreateTweakMenu();
  AddTweakable(m_tweaker, new TweakableBool("Rotate X", &m_rotateX));
  AddTweakable(m_tweaker, new TweakableBool("Rotate Y", &m_rotateY));
}

void GSTeapotBase::OnActive()
{
  GSBase::OnActive();

  m_xRotVel = INITIAL_ROTATE_VEL;
  m_yRotVel = INITIAL_ROTATE_VEL;
}

bool GSTeapotBase::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSTeapotBase::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
