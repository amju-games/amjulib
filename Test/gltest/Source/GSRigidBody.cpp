#include "GSRigidBody.h"
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>
#include "RBBox.h"

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
  AmjuGL::LookAt(0, 20, 20,  0, 0, 0,  0, 1, 0);

  m_rb->Draw();
}

void GSRigidBody::Draw2d()
{
}

void GSRigidBody::OnActive()
{
  GameState::OnActive();
  m_rb = new RBBox;
}

void GSRigidBody::OnCursorEvent(const CursorEvent& ce)
{
  m_point = Vec2f(ce.x, ce.y);
}

void GSRigidBody::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.isDown)
  {
    // Apply force to body
    m_rb->AddForce(Vec3f(0, 100, 0), Vec3f(m_point.x, m_point.y, 0));
  }
}
}
