#include <iostream>
#include "GSClipTest.h"
#include <AmjuGL.h>
#include <ClipLineSegBox.h>
#include <DrawOBB3.h>

namespace Amju
{
GSClipTest::GSClipTest()
{
  //m_nextState=...
}

void GSClipTest::Update()
{
  //GSBase::Update();
}

void GSClipTest::Draw()
{
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));

  GSBase::Draw();

  OBB3 obb3;
  obb3.SetExtents(Vec3f(1, 1, 1));
  static float a = 0;
  a += 0.01f;
  Quaternion q(Vec3f(0, 0, 1), a);
  obb3.SetOrientation(q);

  Vec3f p(-10.0f, 0, 0);
  LineSeg ls(p, -p);
  LineSeg c;
  DrawOBB3(obb3);
  if (Clip(ls, obb3, &c))
  {
std::cout << "Line seg intersects obb3\n";
    PushColour();
    MultColour(Colour(1, 0, 1, 1));
    AmjuGL::DrawLine(AmjuGL::Vec3(c.p0.x, c.p0.y, c.p0.z), AmjuGL::Vec3(c.p1.x, c.p1.y, c.p1.z));
    PopColour();
  } 
  else
  {
    AmjuGL::DrawLine(AmjuGL::Vec3(ls.p0.x, ls.p0.y, ls.p0.z), AmjuGL::Vec3(ls.p1.x, ls.p1.y, ls.p1.z));
  }
}

void GSClipTest::Draw2d()
{
}

void GSClipTest::OnActive()
{
  GSBase::OnActive();
}

bool GSClipTest::OnCursorEvent(const CursorEvent& ce)
{
  GSBase::OnCursorEvent(ce);
  return false;
}

bool GSClipTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  GSBase::OnMouseButtonEvent(mbe);
  return false;
}
} // namespace
