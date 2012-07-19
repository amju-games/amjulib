#include "RBSphere2.h"
#include <AmjuGL.h>
#include "RBManager.h"
#ifdef WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif

namespace Amju
{
RBSphere2::RBSphere2()
{
  m_radius = 1.0f;
}

void RBSphere2::Update()
{
  RigidBody2::Update();
}

void RBSphere2::Draw()
{
  static const int SEGS = 20;
  float a0 = 0;
  float a1 = 0;
  for (int i = 0; i < SEGS; i++)
  {
    a0 = a1;
    a1 += (float)(2.0 * M_PI) / (float)SEGS;
    AmjuGL::DrawLine(
      AmjuGL::Vec3(cos(a0) + m_pos.x, sin(a0) + m_pos.y, 0), 
      AmjuGL::Vec3(cos(a1) + m_pos.x, sin(a1) + m_pos.y, 0));
  }
}

const RBSphere2::TypeName RBSphere2::TYPENAME = "rbsphere2";


void RBSphere2Collision(RBSphere2* s1, RBSphere2* s2)
{

}


static bool regRBSphere2Collision = TheRBManager::Instance()->AddRBFunc(
  RBSphere2::TYPENAME, RBSphere2::TYPENAME, (RBManager::RBFunc)RBSphere2Collision);
}

