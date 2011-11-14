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
#include "RBSceneNode.h"
#include <SceneGraph.h>
#include <OpenGL.h>

#define MAKE_BOX3_DEMO
//#define MAKE_BOX2_DEMO

namespace Amju
{
SceneGraph* GetRBSceneGraph()
{
  static SceneGraph* sg = 0;
  if (!sg)
  {
    sg = new SceneGraph;
  }
  return sg;
}

void AddToSceneGraph(RB* rb)
{
  SceneNode* root = GetRBSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(new RBSceneNode(rb));
}

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

  // TODO Do lighting in shader
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f), // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  GetRBSceneGraph()->Draw();
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

  AddToSceneGraph(rb);
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

  AddToSceneGraph(rb);
}


void GSRigidBody::OnActive()
{
  TheRBManager::Instance()->Clear();

  GSBase::OnActive();
   
  GetRBSceneGraph()->Clear(); 
  GetRBSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

#ifdef MAKE_BOX3_DEMO
  MakeBox3(Vec3f(0, 10.0f, 0));
  MakeBox3(Vec3f(0, 20.0f, 0));

  // Big base
  const float S = 20.0f;
  RBBox3* rb = new RBBox3;
  rb->SetPos(Vec3f(0, -S, 0));
  rb->SetSize(Vec3f(S, S, S));
  rb->SetInvMass(0); // immovable
  TheRBManager::Instance()->AddRB(rb);
  AddToSceneGraph(rb);
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
  AddToSceneGraph(rb);
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
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == '2')
  {
    MakeBox2(Vec2f(3.0f, 20.0), 0.3f);
  }
  else if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR && ke.key == '3')
  {
    MakeBox3(Vec3f(3.0f, 20.0, 0));
  }

  return true; // handled
}

bool GSRigidBody::OnCursorEvent(const CursorEvent& ce)
{
  GSBase::OnCursorEvent(ce);
  m_point = Vec2f(ce.x, ce.y);
  return false;
}

bool GSRigidBody::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  GSBase::OnMouseButtonEvent(mbe);
  if (mbe.isDown)
  {
    // Apply force to body
    //m_rb->AddForce(Vec3f(0, 100, 0), Vec3f(m_point.x, m_point.y, 0));
  }
  return false;
}
}
