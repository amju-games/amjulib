#include "GSLetterCancellation1.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <ResourceManager.h>
#include <EventPoller.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include <SoundManager.h>
#include "ROConfig.h"
#include "GSCogTestMenu.h"
#include "CogTestResults.h"
#include "LurkMsg.h"
#include "GSMain.h"
#include "GameConsts.h"

namespace Amju
{
static const int MAX_LETTERS = 6 * 52; // size of grid, from Malec et al
//static const int NUM_M = 32; // from Malec et al, number of letters in grid to find
static const float MAX_TIME = 180.0f; // 3 mins, from Malec et al

static void OnDoneButton()
{
  TheGSLetterCancellation1::Instance()->FinishedTest();
}

static void StartLC()
{
  TheGSLetterCancellation1::Instance()->StartTest();
}

void GSLetterCancellation1::FinishedTest()
{
  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetVisible(false);

  bool isPrac = TheGSCogTestMenu::Instance()->IsPrac();
  if (isPrac)
  {
    TheGSCogTestMenu::Instance()->SetIsPrac(false);

    std::string str;
    if (m_correct == 0)
    {
      // TODO offer another practice go
      str = "Oh dear, you didn't get any correct! Well, I am sure you will do better this time!";

      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, StartLC);
      TheLurker::Instance()->Queue(lm);
    }
    else
    {
      TheSoundManager::Instance()->PlayWav("Sound/applause3.wav");

      str = "OK, you got " + ToString(m_correct) +
        " correct! Let's try it for real! You have 3 minutes!";

      LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE, StartLC);
      TheLurker::Instance()->Queue(lm);
    }
  }
  else
  {
    // Send results, go to next test.
    std::string str;
    if (m_correct == 0)
    {
      str = "Oh dear, you didn't get any correct! Well, I am sure you will do better next time!";
    }
    else
    {
      str = "Well done, you finished! You got " + ToString(m_correct) + " correct!";
      TheSoundManager::Instance()->PlayWav("Sound/applause3.wav");
    }
    LurkMsg lm(str, LURK_FG, LURK_BG, AMJU_CENTRE);
    TheLurker::Instance()->Queue(lm);

    TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "correct", ToString(m_correct)));
    TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "incorrect", ToString(m_incorrect)));
    TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "time", ToString(m_timer)));
  
    TheGSCogTestMenu::Instance()->AdvanceToNextTest();

    TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
  }
}

static void OnCancelButton()
{
  // TODO Should we allow this ?

  // TODO Where should we go when we administer the test for real ?
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
}

bool LCListener::OnCursorEvent(const CursorEvent& ce)
{
  return TheGSLetterCancellation1::Instance()->OnCursorEvent(ce);
}

bool LCListener::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return TheGSLetterCancellation1::Instance()->OnMouseButtonEvent(mbe);
}

GSLetterCancellation1::GSLetterCancellation1()
{
  m_listener = new LCListener;

  m_top = 0.5f; 
  m_left = -1.0f; 
  m_vSpacing = 0.12f;
  m_hSpacing = 0.0325f;
  m_fontSize = 0.05f;
  m_sqSize = 0.025f;

  m_mouseOver = false;

  m_timer = MAX_TIME;
  m_correct = 0;
  m_incorrect = 0;

  m_showLurk = true;
}

Rect GSLetterCancellation1::MakeRect(int i, int j)
{
  float x = m_left + (float)j * m_hSpacing;
  float y = m_top - (float)i * m_vSpacing;
  return Rect(x, x + m_sqSize, y, y + m_sqSize + m_sqSize);
}

bool GSLetterCancellation1::OnCursorEvent(const CursorEvent& ce)
{
  // Highlight letter under cursor
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 52; j++)
    {
      Rect r = MakeRect(i, j);
      if (r.IsPointIn(Vec2f(ce.x, ce.y)))
      {
        m_mouseRect = r;
        m_mouseOver = true;
        return true;
      }
    }
  }
  m_mouseOver = false;

  return false;
}

bool GSLetterCancellation1::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.isDown)
  {
    return false;
  }

  bool isPrac = TheGSCogTestMenu::Instance()->IsPrac();

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 52; j++)
    {
      float x = m_left + (float)j * m_hSpacing;
      float y = m_top - (float)i * m_vSpacing;
      Rect r = MakeRect(i, j);

      if (r.IsPointIn(Vec2f(mbe.x, mbe.y)))
      {
std::cout << "sel:" << m_letters[i][j] << "\n";
        if ('@' == m_letters[i][j])
        {
          // This symbol means already picked this letter
          return false;
        }

        if (m_specialLetter == m_letters[i][j])
        {
          if (isPrac && m_correct == 0)
          {
            LurkMsg lm("Yes, that's correct!", LURK_FG, LURK_BG, AMJU_TOP);
            TheLurker::Instance()->Queue(lm);
          }

          TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-cogtest-correct"));
          m_correct++;
          if (m_correct == m_numSpecialLetter)
          {
            // Done!
            OnDoneButton();
          }
        }
        else
        {
          if (isPrac && m_incorrect == 0)
          {
            LurkMsg lm("Whoops, that one is not correct!", 
              LURK_FG, LURK_BG, AMJU_TOP);
            TheLurker::Instance()->Queue(lm);
          }

          TheSoundManager::Instance()->PlayWav(ROConfig()->GetValue("sound-cogtest-fail"));
          m_incorrect++;
        }

        // Mark as picked
        m_letters[i][j] = '@';

        GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "score");
        std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
        scoreText->SetText(s); 

        AmjuGL::Vert verts[4] = 
        {
          AmjuGL::Vert(x + m_sqSize, y, 0,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz  
          AmjuGL::Vert(x + m_sqSize, y + 2 * m_sqSize, 0,   1, 1,   0, 1, 0),
          AmjuGL::Vert(x, y + 2 * m_sqSize, 0,   0, 1,   0, 1, 0),
          AmjuGL::Vert(x, y, 0,   0, 0,   0, 1, 0)
        };

        AmjuGL::Tri tri;
        tri.m_verts[0] = verts[0];
        tri.m_verts[1] = verts[1];
        tri.m_verts[2] = verts[2];
        m_blocks.push_back(tri);

        tri.m_verts[0] = verts[0];
        tri.m_verts[1] = verts[2];
        tri.m_verts[2] = verts[3];
        m_blocks.push_back(tri);

        // Done
        return false;
      }
    }
  }
  return false;
}

void GSLetterCancellation1::Update()
{
  GSGui::Update();

  m_timer -= TheTimer::Instance()->GetDt();

  bool isPrac = TheGSCogTestMenu::Instance()->IsPrac();
  // Don't do countdown if in practice mode
  if (!isPrac)
  {
    GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");
    std::string s;
    if (m_timer > 0)
    {
      int min = (int)(m_timer / 60.0f);
      int sec = (int)(m_timer - 60.0f * min);
      s = ToString(min) + ":" + (sec < 10 ? "0" : "") + ToString(sec);
    }
    else
    {
      m_isFinished = true;
      // TODO flash
      s = "0:00";

      //if (m_timer < -10.0f)
      {
        OnDoneButton();
      }
    }
    timeText->SetText(s);
  }
}

void GSLetterCancellation1::Draw()
{
  // No funky BG
  //GSGui::Draw();
}

void GSLetterCancellation1::Draw2d()
{
  PushColour();
  AmjuGL::SetColour(Colour(0, 0, 0, 1));
  m_ts.Bind();
  for (int i = 0; i < 6; i++)
  {
    AmjuGL::DrawTriList(m_tris[i]);
  }

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  if (m_mouseOver)
  {
    AmjuGL::SetColour(Colour(0, 1, 1, 0.5f));
    DrawSolidRect(m_mouseRect);
  }

  // Highlight selected letters
  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  if (!m_blocks.empty())
  {
    AmjuGL::DrawTriList(m_blocks);
  }

#ifdef SHOW_SELECTION_RECTS
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 52; j++)
    {
      Rect r = MakeRect(i, j);
      DrawRect(r);
    }
  }
#endif

  PopColour();

  // Superimpose cursor
  GSGui::Draw2d();
}

bool GSLetterCancellation1::LoadConfig(int testId, const std::string& filename)
{
  m_testId = testId;

  File f;
  if (!f.OpenRead(filename))
  {
    // Oh no, this is serious. If we can't load the tests, we can't do the research.
    // TODO Display a special help screen.
    // TODO Send a log message to the server: ReportError (and Assert) should do this.

    return false;
  }

  // Get font image filename
  if (!f.GetDataLine(&m_fontImgFilename))
  {
    f.ReportError("Expected font filename for letter cancellation test");
    return false;
  } 

  // Load special letter
  std::string s;
  if (!f.GetDataLine(&s))
  {
    f.ReportError("Expected special letter");
    return false;
  }
  m_specialLetter = s[0];

  // Get number of special letters sprinkled in grid
  if (!f.GetInteger(&m_numSpecialLetter))
  {
    f.ReportError("Expected number of special letters");
    return false;
  }

  // Get the non-special letters
  if (!f.GetDataLine(&m_noSpecial))
  {
    f.ReportError("Expected non-special characters");
    return false;  
  }

  if (!f.GetFloat(&m_top)) 
  {
    f.ReportError("Expected m_top");
    return false;  
  }

  if (!f.GetFloat(&m_left))
  {
    f.ReportError("Expected m_left");
    return false;  
  }

  if (!f.GetFloat(&m_vSpacing))
  {
    f.ReportError("Expected m_vSpacing");
    return false;  
  }

  if (!f.GetFloat(&m_hSpacing))
  {
    f.ReportError("Expected m_hSpacing");
    return false;  
  }

  if (!f.GetFloat(&m_fontSize))
  {
    f.ReportError("Expected m_fontSize");
    return false;  
  }

  if (!f.GetFloat(&m_sqSize))
  {
    f.ReportError("Expected m_sqSize");
    return false;  
  }

  return true;
}

void GSLetterCancellation1::OnActive()
{
  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-lettercancellation.txt");
  Assert(m_gui);
  GuiComposite* comp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  Assert(comp);

  // TODO just once
  // TODO font file name - get from font mgr ?
  if (!m_ts.Load(m_fontImgFilename, 16, 8, m_fontSize, m_fontSize))
  {
    ReportError("Failed to load font for letter cancellation test");
    Assert(0);
    // ?
  }

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnDoneButton);
  done->SetHasFocus(true);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  TheEventPoller::Instance()->AddListener(m_listener);

  StartTest();
}

void GSLetterCancellation1::StartTest()
{
  // Reset timer and score
  m_timer = MAX_TIME;
  m_mouseOver = false;
  m_correct = 0;
  m_incorrect = 0;
  m_blocks.clear();

  bool isPrac = TheGSCogTestMenu::Instance()->IsPrac();
  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");

  // Always allow player to escape, but TODO no prize if you skip out early
  done->SetVisible(true);

  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");
  timeText->SetVisible(!isPrac);

  GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "score");
  std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
  scoreText->SetText(s);

  // Create a 6 * 52 grid of letters. There are 32 'M's randomly distributed.
  // (Generally: there are m_numSpecialLetters * m_specialLetter).
  // The participant should click on as many of the special letters  as he/she can in 3 mins.
  // NB How do we deal with wrong clicks, etc ?

  // This is all the letters in the grid, which is eventually shuffled
  char letters[MAX_LETTERS + 1];

  // First we get the exact required number of the special letter.
  for (int i = 0; i < m_numSpecialLetter; i++)
  {
    letters[i] = m_specialLetter;
  }

  // These letters are used to randomly fill the rest of the space.
  //std::string no_m = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // will have no "m" (whatever the special letter is)
  //no_m.erase(std::remove(no_m.begin(), no_m.end(), m_specialLetter), no_m.end());
std::cout << "Letters but with no special letter: " << m_noSpecial << "\n";

  int size = m_noSpecial.size();
  for (int i = m_numSpecialLetter; i < MAX_LETTERS; i++)
  {
    letters[i] = m_noSpecial[rand() % size];
  }

  // Letters now consists of the exact number of special characters, followed by a bunch of random
  //  letters (which doesn't include any of the special letter). Now we shuffle.
  std::random_shuffle(letters, letters + MAX_LETTERS);

  for (int i = 0; i < 6; i++)
  {
    // Make one row of tris
    m_tris[i].clear();

    for (int j = 0; j < 52; j++)
    {
      AmjuGL::Tri tris[2]; // 2 tris for each character
      char c = letters[i * 52 + j]; // this character
      m_letters[i][j] = c; // store for checking later

      // Make 2 textured tris to draw this character
      m_ts.MakeTris(c - ' ', 1.0f, tris, 
        m_left + m_hSpacing * (float)j, m_top - m_vSpacing * (float)i);

      m_tris[i].push_back(tris[0]);
      m_tris[i].push_back(tris[1]);
    }
  }
}

void GSLetterCancellation1::OnDeactive()
{
  GSGui::OnDeactive();
  TheEventPoller::Instance()->RemoveListener(m_listener);
  // TODO Kill font texture
}

} // namespace
