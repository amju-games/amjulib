#include <ResourceManager.h>
#include <AmjuRand.h>
#include <Screen.h>
#include <EventPoller.h>
#include <GuiText.h>
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

GSTrailMakingTest::GSTrailMakingTest()
{
  m_listener = new TrailListener;

  m_currentCircle = -1;
  m_correct = 0;
}

void GSTrailMakingTest::OnDeactive()
{
  GSCogTestBase::OnDeactive();
  TheEventPoller::Instance()->RemoveListener(m_listener);
}

void GSTrailMakingTest::OnActive()
{
  GSCogTestBase::OnActive();


  TheEventPoller::Instance()->AddListener(m_listener, 100);
  // high number = low priority, so GUI button clicks etc eat the events.

  m_gui = LoadGui("gui-trailmaking.txt"); 
  Assert(m_gui);

  // Create dots
  for (int i = 0; i < 25; i++)
  {
    Vec2f pos(Rnd(-0.7f, 0.7f), Rnd(-0.7f, 0.7f));
    std::string str = ToString(i);
    // TODO A/1/B/2 version
    TrailCircle d(pos, str);
    m_circles.push_back(d);
  }

  // Reset test scores
  m_currentCircle = -1;
  m_correct = 0;
}

void GSTrailMakingTest::Update()
{
  GSCogTestBase::Update();
}

void GSTrailMakingTest::Draw()
{
  DrawCogTestBg();
}

static const float CIRCLE_SIZE = 0.12f;

bool TrailCircle::IsClicked(const Vec2f& cursorPos)
{
  float sqDist = (m_pos - cursorPos).SqLen();
  bool ret = (sqDist < CIRCLE_SIZE * CIRCLE_SIZE);
  return ret;    
}

void TrailCircle::Draw()
{
  static GuiImage* circleImg = 0;
  static GuiText text;

  if (!circleImg)
  {
    circleImg = new GuiImage;
    Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes("circ1.png");
    tex->SetFilter(AmjuGL::AMJU_TEXTURE_NICE);
    circleImg->SetTexture(tex);
    float aspect = (float)Screen::X() / (float)Screen::Y();
    circleImg->SetSize(Vec2f(CIRCLE_SIZE, CIRCLE_SIZE * aspect));

    text.SetSize(Vec2f(0.1f, 0.1f));
  }

  Colour col(1, 1, 1, 1);
  if (m_clicked)
  {
    col = Colour(0, 0, 0, 1);
  }

  circleImg->SetLocalPos(Vec2f(m_pos.x - CIRCLE_SIZE * 0.5f, m_pos.y + CIRCLE_SIZE * 0.5f));
  PushColour();
  circleImg->Draw();
  PopColour();

  // Draw number/letter
  text.SetSize(0.2f, 0.1f);
  text.SetJust(GuiText::AMJU_JUST_CENTRE);
  text.SetLocalPos(m_pos + Vec2f(-0.1f, 0.03f));
  text.SetText(m_str);
  text.Draw();
}

void GSTrailMakingTest::Draw2d()
{
  for (unsigned int i = 0; i < m_circles.size(); i++)
  {
    m_circles[i].Draw();
  }

  // Draw lines connecting correctly clicked dots
  PushColour();
  AmjuGL::SetColour();
  for (int i = 1; i < m_currentCircle; i++)
  {
    Vec2f pos[2] = { m_circles[i - 1].m_pos, m_circles[i].m_pos };
    AmjuGL::DrawLine(AmjuGL::Vec3(pos[0].x, pos[0].y, 0), AmjuGL::Vec3(pos[1].x, pos[1].y, 0));
  }
  PopColour();

  GSBase::Draw2d(); // cursor
}

bool GSTrailMakingTest::OnCursorEvent(const CursorEvent& ce)
{
//std::cout << "Cursor pos: (" << ce.x << ", " << ce.y << ")\n";

  // Check if cursor is on a dot
  for (int i = 0; i < (int)m_circles.size(); i++)
  {
    if (m_circles[i].IsClicked(Vec2f(ce.x, ce.y)))
    {
std::cout << "Clicked on circle " << i << "\n";

      if (i == m_currentCircle)
      {
        // On current dot - do nothing?
      }
      else if (i == m_currentCircle + 1)
      {
        // On next dot in sequence - correct!
        m_correct++;
        m_currentCircle++;

std::cout << "CORRECT! Now on to circle " << m_currentCircle << "\n";
      }
      else
      {
        // On a different dot. Incorrect? But only score incorrect once.
      }
    }
  }

  return true;
}

bool GSTrailMakingTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return true;
}
}
