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
#include "GSCogTestMenu.h"
#include "CogTestResults.h"

namespace Amju
{
static const int MAX_LETTERS = 6 * 52; // size of grid, from Malec et al
//static const int NUM_M = 32; // from Malec et al, number of letters in grid to find
static const float MAX_TIME = 180.0f;

static void OnDoneButton()
{
  TheGSLetterCancellation1::Instance()->FinishedTest();
}

void GSLetterCancellation1::FinishedTest()
{
  // Send results, go to next test.
  // NB We must be sure to not lose the results. Store them locally, keep sending until we get ack 
  //  from server.

std::cout << "Finished! Sending results to server...\n";

  TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "correct", ToString(m_correct)));
  TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "incorrect", ToString(m_incorrect)));
  TheCogTestResults::Instance()->StoreResult(new Result(m_testId, "time", ToString(m_timer)));
  
  // TODO Where should we go when we administer the test for real ?
  TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
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

  m_fontImgFilename = "font2d/arial_round_1024_36pt.png";
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
        if (m_specialLetter == m_letters[i][j])
        {
          // TODO sound, but pre-load into mem
          m_correct++;
          if (m_correct == m_numSpecialLetter)
          {
            // Done!
            OnDoneButton();
          }
        }
        else
        {
          // TODO sound, but pre-load into mem
          m_incorrect++;
        }

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

        break;
      }
    }
  }
  return false;
}

void GSLetterCancellation1::Update()
{
  GSGui::Update();

  m_timer -= TheTimer::Instance()->GetDt();

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
  // Reset timer and score
  m_timer = MAX_TIME;
  m_mouseOver = false;
  m_correct = 0;
  m_incorrect = 0;
  m_blocks.clear();

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

  // TODO Set focus element, cancel element, command handlers

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnDoneButton);
  done->SetHasFocus(true);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  TheEventPoller::Instance()->AddListener(m_listener);
}

void GSLetterCancellation1::OnDeactive()
{
  GSGui::OnDeactive();
  TheEventPoller::Instance()->RemoveListener(m_listener);
  // TODO Kill font texture
}

} // namespace