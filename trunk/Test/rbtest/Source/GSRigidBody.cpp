#include "GSRigidBody.h"
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>
#include <DegRad.h>
#include <AmjuGL.h>
#include "RBBox.h"
#include "RBSphere.h"

namespace Amju
{
GSRigidBody::GSRigidBody()
{
}

void GSRigidBody::Update()
{
  m_rb->Update();
}

void GSRigidBody::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  GSBase::Draw();

  m_rb->Draw();
}

void GSRigidBody::Draw2d()
{
}

void GSRigidBody::OnActive()
{
  GSBase::OnActive();
  m_rb = new RBBox;
  m_rb->SetPos(Vec3f(0, 5.0f, 0));
  Quaternion q;
  q.SetAxisAngle(DegToRad(30.0f), Vec3f(0, 0, 1.0f));
  m_rb->SetRot(q);
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
