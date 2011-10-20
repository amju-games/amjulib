#include "GSRigidBody.h"
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>
#include <DegRad.h>
#include <AmjuGL.h>
#include "RBBox.h"
#include "RBSphere.h"
#include "RBManager.h"

namespace Amju
{
GSRigidBody::GSRigidBody()
{
  m_paused = false;
}

void GSRigidBody::Update()
{
  if (!m_paused)
  {
    TheRBManager::Instance()->Update();
  }
}

void GSRigidBody::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  GSBase::Draw();

  TheRBManager::Instance()->Draw();
}

void GSRigidBody::Draw2d()
{
}

void MakeBox(const Vec3f& pos)
{
  RBBox* rb = new RBBox;
  rb->SetPos(pos);
  rb->SetSize(Vec3f(
    (float)(rand() % 2 + 1), 
    (float)(rand() % 3 + 1),
    (float)(rand() % 4 + 1)));
  // Work out mass from volume ?

  Quaternion q;
  float angle = (float)(rand() % 360);
  std::cout << "Angle :" << angle << " degs\n";
  q.SetAxisAngle(DegToRad(angle), Vec3f(0, 0, 1.0f));
  rb->SetRot(q);
  TheRBManager::Instance()->AddRB(rb);
}

void GSRigidBody::OnActive()
{
  TheRBManager::Instance()->Clear();

  GSBase::OnActive();

  MakeBox(Vec3f(0, 10.0f, 0));
//  MakeBox(Vec3f(0, 20.0f, 0));

  // Big base
  const float S = 20.0f;
  RBBox* rb = new RBBox;
  rb->SetPos(Vec3f(0, -S, 0));
  rb->SetSize(Vec3f(S, S, S));
  rb->SetInvMass(0); // immovable
  TheRBManager::Instance()->AddRB(rb);

}

bool GSRigidBody::OnKeyEvent(const KeyEvent& ke)
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'r')
  {
    // Reset sim
    OnActive();
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == 'p')
  {
    m_paused = !m_paused;
  }

  return true; // handled
}

bool GSRigidBody::OnCursorEvent(const CursorEvent& ce)
{
  m_point = Vec2f(ce.x, ce.y);
  return false;
}

bool GSRigidBody::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.isDown)
  {
    // Apply force to body
    //m_rb->AddForce(Vec3f(0, 100, 0), Vec3f(m_point.x, m_point.y, 0));
  }
  return false;
}
}
