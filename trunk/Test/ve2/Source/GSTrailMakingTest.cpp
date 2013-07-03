#include <ResourceManager.h>
#include <AmjuRand.h>
#include "GSTrailMakingTest.h"

namespace Amju
{
class TrailListener : public EventListener
{
public:
  virtual bool OnCursorEvent(const CursorEvent& ce)
  {
    return TheGSTrailMakingTest::Instance()->OnCursorEvent(ce);
  }

  virtual bool OnMouseButtonEvent(const MouseButtonEvent& mbe)
  {
    return TheGSTrailMakingTest::Instance()->OnMouseButtonEvent(mbe);
  }
};

void GSTrailMakingTest::OnActive()
{
  GSCogTestBase::OnActive();

  m_gui = LoadGui("gui-trailmaking.txt"); 
  Assert(m_gui);

  // Create dots
  for (int i = 0; i < 25; i++)
  {
    Vec2f pos(Rnd(-0.7f, 0.7f), Rnd(-0.7f, 0.7f));
    std::string str = ToString(i);
    // TODO A/1/B/2 version
    TrailDot d(pos, str);
    m_dots.push_back(d);
  }

}

void GSTrailMakingTest::Update()
{
  GSCogTestBase::Update();
}

void GSTrailMakingTest::Draw()
{
  DrawCogTestBg();
}

void TrailDot::Draw()
{
  static const float CIRCLE_SIZE = 0.2f;
  static GuiImage* circleImg = 0;
  if (!circleImg)
  {
    circleImg = new GuiImage;
    circleImg->SetTexture((Texture*)TheResourceManager::Instance()->GetRes("circ1.png"));
    circleImg->SetSize(Vec2f(CIRCLE_SIZE, CIRCLE_SIZE));
  }
  Colour col(1, 1, 1, 1);
  if (m_clicked)
  {
    col = Colour(0, 0, 0, 1);
  }
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_pos.x - CIRCLE_SIZE * 0.5f, m_pos.y + CIRCLE_SIZE * 0.5f, 0);
  PushColour();
  circleImg->Draw();
  PopColour();
  // TODO Draw character
  AmjuGL::PopMatrix();

}

void GSTrailMakingTest::Draw2d()
{
  for (unsigned int i = 0; i < m_dots.size(); i++)
  {
    m_dots[i].Draw();
  }

  GSBase::Draw2d();
}

bool GSTrailMakingTest::OnCursorEvent(const CursorEvent& ce)
{
  return true;
}

bool GSTrailMakingTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return true;
}
}
