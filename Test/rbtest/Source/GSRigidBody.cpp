#include "GSRigidBody.h"
#include <iostream>
#include <Game.h>
#include <AmjuGL.h>
#include <Font.h>
#include <StringUtils.h>
#include <DegRad.h>
#include <AmjuGL.h>
#include "RBBox3.h"
#include "RBBox2.h"
#include "RBSphere3.h"
#include "RBSphere2.h"
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

void MakeBox2(const Vec2f& pos, float rads)
{
  RBBox2* rb = new RBBox2;
  rb->SetPos(pos);
  rb->SetSize(Vec2f( //(2, 2));
    (float)(rand() % 2 + 1), 
    (float)(rand() % 3 + 1)));
//  float angle = (float)rand() / (float)RAND_MAX * M_PI;
  rb->SetRot(rads);
  TheRBManager::Instance()->AddRB(rb);
}

void MakeBox3(const Vec3f& pos)
{
  RBBox3* rb = new RBBox3;
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

//#define MAKE_BOX3_DEMO
#define MAKE_BOX2_DEMO

void GSRigidBody::OnActive()
{
  TheRBManager::Instance()->Clear();

  GSBase::OnActive();

#ifdef MAKE_BOX3_DEMO
  MakeBox3(Vec3f(0, 10.0f, 0));
//  MakeBox3(Vec3f(0, 20.0f, 0));

  // Big base
  const float S = 20.0f;
  RBBox3* rb = new RBBox3;
  rb->SetPos(Vec3f(0, -S, 0));
  rb->SetSize(Vec3f(S, S, S));
  rb->SetInvMass(0); // immovable
  TheRBManager::Instance()->AddRB(rb);
#endif

#ifdef MAKE_BOX2_DEMO
  // Box 2D demo
  MakeBox2(Vec2f(0, 10.0f), 0.2f);
  MakeBox2(Vec2f(3.0f, 20.0), 0.3f);
  MakeBox2(Vec2f(2.0f, 30.0), 0.4f);

  // Sphere 2
  RBSphere2* rbs2 = new RBSphere2;
  rbs2->SetPos(Vec2f(5, 5));
//  TheRBManager::Instance()->AddRB(rbs2);

  // Base 
  const float S = 20.0f;
  RBBox2* rb = new RBBox2;
  rb->SetPos(Vec2f(0, -S));
  rb->SetSize(Vec2f(S, S));
  rb->SetInvMass(0); // immovable
  TheRBManager::Instance()->AddRB(rb);
#endif
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
