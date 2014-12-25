#include <Game.h>
#include <DrawOBB2.h>
#include <DrawOBB3.h>
#include <Colour.h>
#include <AmjuGL.h>
#include <Timer.h>
#include "GSObb2Test.h"
#include "GSLighting.h"

namespace Amju
{
GSObb2Test::GSObb2Test()
{
  m_name = "OBB2";
  m_nextState = TheGSLighting::Instance();
}

void GSObb2Test::Update()
{
  GSBase::Update();

  Vec2f v = m_obbs[1].GetCentre();
  v.x -= 0.001f;
  m_obbs[1].SetCentre(v);

  static float rads = 0;
  rads += TheTimer::Instance()->GetDt();
  m_obbs[0].SetRotation(rads);
}

void GSObb2Test::DrawScene()
{
  AmjuGL::SetClearColour(Colour(0.2f, 0.2f, 0.2f, 1));
  GSBase::DrawHelp();

/* 
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(10, 0, 0,  0, 0, 0,  0, 1, 0);

  static OBB3 b1(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(1, 2, 3));
  static OBB3 b2(Vec3f(0, 0, 10), Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(1, 1, 1));
  static float a = 0;
  a += 0.001f;
  b1.SetOrientation(Quaternion(Vec3f(1, 0, 0), a));
  b2.SetOrientation(Quaternion(Vec3f(0, 1, 0), a));
  Vec3f c = b2.GetCentre();
  c.z -= 0.001f;
  b2.SetCentre(c);

  PushColour();
  if (Intersects(b1, b2))
  {
    MultColour(Colour(1, 0, 0, 1));
  }
  DrawOBB3(b1);
  DrawOBB3(b2);
  PopColour();
*/
}

void GSObb2Test::Draw2d()
{
  GSBase::Draw2d();

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  PushColour();
  if (Intersects(m_obbs[0], m_obbs[1]))
  {
    MultColour(Colour(1, 0, 0, 1));
  }

  for (unsigned int i = 0; i < m_obbs.size(); i++)
  {
    DrawSolidOBB2(m_obbs[i]);
  }

  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

void GSObb2Test::OnActive()
{
  GSBase::OnActive();

  OBB2 obb1(Vec2f(0, 0), Vec2f(1, 0), Vec2f(0.2f, 0.2f));
  m_obbs.push_back(obb1);

  OBB2 obb2(Vec2f(0.6f, 0), Vec2f(1, 1), Vec2f(0.2f, 0.2f));
  m_obbs.push_back(obb2);

  //// Populate OBBS
  //for (int i = 0; i < 1; i++)
  //{
  //  OBB2 obb2(Vec2f(0, 0), Vec2f(1, 1), Vec2f(0.2f, 0.2f));
  //  m_obbs.push_back(obb2);
  //}
}
}
