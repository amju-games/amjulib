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

  m_alternatingVersion = true;
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
  LoadCommonGui();

  // Grid of allowable positions
  static const int GRID_W = 8;
  static const int GRID_H = 7;
  float w = 2.0f / GRID_W;
  float h = 1.7f / GRID_H; // top of screen has info/buttons

#ifdef RANDOM_DISTRIBUTION
  Vec2f grid[GRID_W * GRID_H];

  int g = 0;
  for (int i = 0; i < GRID_H; i++)
  {
    for (int j = 0; j < GRID_W; j++)
    {
      grid[g++] = Vec2f(j * w - 0.9f, i * h - 0.9f);
    }
  }
  std::random_shuffle(grid, grid + GRID_W * GRID_H);

  // Create circles
  g = 0;
  for (int i = 0; i < 25; i++)
  {
    Vec2f pos = grid[g++] + Vec2f(Rnd(-0.05f, 0.05f), Rnd(-0.05f, 0.05f));
    AddCircle(i, pos);
  }
#endif // RANDOM_DISTRIBUTION

  // Kind of Random walk
  // Array of bools so we don't have 2 circles overlapping
  bool grid[GRID_W * GRID_H];
  for (int i = 0; i < GRID_W * GRID_H; i++)
  {
    grid[i] = false;
  }

OH_CRAP:

  // Start in the middle
  Vec2i pos(GRID_W / 2, GRID_H / 2);
  for (int i = 0; i < 25; i++)
  {
    Vec2i newPos = pos;
    int count = 0;
    int gridSq = newPos.x * GRID_W + newPos.y;
    while (grid[gridSq])
    {
      count++;
      if (count > 50)
      {
std::cout << "Oh crap, got boxed in, restarting random walk..\n";

        goto OH_CRAP; // I R teh awsom programmer
      }

      // reach further if we can't find an empty square
      int reach = (count < 10) ? 3 : 5;

      // Find a new position close to the old position
      newPos = Vec2i(pos.x + rand() % reach - reach/2, 
        pos.y + rand() % reach - reach/2);
      if (newPos.x < 0) newPos.x = 0;
      if (newPos.x >= GRID_W) newPos.x = GRID_W - 1;
      if (newPos.y < 0) newPos.y = 0;
      if (newPos.y >= GRID_H) newPos.y = GRID_H - 1;
    
      gridSq = newPos.x * GRID_W + newPos.y;
    }
    pos = newPos;

    grid[gridSq] = true; // mark grid square as used

    // Convert grid coord to screen coord, with a bit of randomness
    Vec2f posf = Vec2f(pos.x * w - 0.9f + Rnd(-0.05f, 0.05f), 
      pos.y * h - 0.9f + Rnd(-0.05f, 0.05f));

    AddCircle(i, posf);
  }

  // Reset test scores
  m_currentCircle = -1;
  m_correct = 0;
}

void GSTrailMakingTest::AddCircle(int i, const Vec2f& pos)
{
    std::string str;
    if (m_alternatingVersion)
    {
      // A/1/B/2 version
      int j = i / 2;
      if (i % 2)
      {
        str = std::string(1, 'A' + j);
      }
      else
      {
        str = ToString(j + 1);
      }
    }
    else
    {
      str = ToString(i);
    }

    TrailCircle d(pos, str);
    m_circles.push_back(d);
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

  const Colour BLACK(0, 0, 0, 1);
  const Colour WHITE(1, 1, 1, 1);
  const Colour RED(1, 0, 0, 1);

  circleImg->SetLocalPos(Vec2f(m_pos.x - CIRCLE_SIZE * 0.5f, m_pos.y + CIRCLE_SIZE * 0.5f));
  PushColour();
  MultColour(m_incorrect ? RED : (m_clicked ? BLACK : WHITE));
  circleImg->Draw();
  PopColour();

  // Draw number/letter
  text.SetSize(Vec2f(0.2f, 0.1f));
  text.SetJust(GuiText::AMJU_JUST_CENTRE);
  text.SetLocalPos(m_pos + Vec2f(-0.1f, 0.012f));
  text.SetText(m_str);
  text.SetFgCol(m_clicked || m_incorrect ? WHITE : BLACK);
  text.Draw();
}

void GSTrailMakingTest::Draw2d()
{
  for (unsigned int i = 0; i < m_circles.size(); i++)
  {
    m_circles[i].Draw();
  }

  // Draw lines connecting correctly clicked dots
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  PushColour();
  AmjuGL::SetColour(Colour(0, 0, 0, 1));
  for (int i = 1; i < m_currentCircle; i++)
  {
    Vec2f pos[2] = { m_circles[i - 1].m_pos, m_circles[i].m_pos };
    AmjuGL::DrawLine(AmjuGL::Vec3(pos[0].x, pos[0].y, 0), AmjuGL::Vec3(pos[1].x, pos[1].y, 0));
  }
  PopColour();

  GSCogTestBase::Draw2d(); // cursor
}

bool GSTrailMakingTest::OnCursorEvent(const CursorEvent& ce)
{
//std::cout << "Cursor pos: (" << ce.x << ", " << ce.y << ")\n";

  for (int i = 0; i < (int)m_circles.size(); i++)
  {
    m_circles[i].m_incorrect = false;
  }

  // Check if cursor is on a dot
  bool noCircle = true;
  for (int i = 0; i < (int)m_circles.size(); i++)
  {
    if (m_circles[i].IsClicked(Vec2f(ce.x, ce.y)))
    {
      noCircle = false;

//std::cout << "Clicked on circle " << i << "\n";

      if (i == m_currentCircle)
      {
        // On current dot - do nothing?
      }
      else if (i == m_currentCircle + 1)
      {
        // On next dot in sequence - correct!
        m_correct++;
        m_currentCircle++;
        m_circles[m_currentCircle].m_clicked = true;

        if (m_currentCircle >= (int)m_circles.size())
        {
std::cout << "FINISHED!\n";
          // Save num errors, time, num completed, (other info ?)
        }
        else
        {
std::cout << "CORRECT! Now on to circle " << m_currentCircle + 1 << "\n";
        }
      }
      else
      {
        // On a different dot. Incorrect? But only score incorrect once.
        if (!m_circles[i].m_incorrect)
        {
          // TODO Get this to print out only once per incorrect circle,
          //  then can use for incorrect score.
////          std::cout << "Incorrect!\n";
        }
        m_circles[i].m_incorrect = true;
      }
    }
  }
  //if (noCircle)
  //{
  //  for (int i = 0; i < (int)m_circles.size(); i++)
  //  {
  //    m_circles[i].m_incorrect = false;
  //  }
  //}

  return true;
}

bool GSTrailMakingTest::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return true;
}
}
