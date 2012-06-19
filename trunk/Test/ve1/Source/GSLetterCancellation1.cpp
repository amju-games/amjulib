#include "GSLetterCancellation1.h"
#include <AmjuGL.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <ResourceManager.h>
#include <EventPoller.h>
#include <Game.h>
#include <DrawRect.h>
#include "GSCogTestMenu.h"

namespace Amju
{
static void OnCancelButton()
{
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
  m_hSpacing = 0.03f;
  m_fontSize = 0.05f;
  m_sqSize = 0.025f;
}

Rect GSLetterCancellation1::MakeRect(int i, int j)
{
  float x = m_left + (float)j * m_hSpacing;
  float y = m_top - (float)i * m_vSpacing;
  return Rect(x, x + m_sqSize, y, y + m_sqSize + m_sqSize);
}

bool GSLetterCancellation1::OnCursorEvent(const CursorEvent& ce)
{
  /*
  // TEST
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 52; j++)
    {
      Rect r = MakeRect(i, j);
      // TODO highlight any selected letter    
      if (r.IsPointIn(Vec2f(ce.x, ce.y)))
      {
        std::cout << m_letters[i][j];
        break;
      }
    }
  }
  */

  return false;
}

bool GSLetterCancellation1::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
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

  // Highlight selected letters
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
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

void GSLetterCancellation1::OnActive()
{
  // Reset timer and score

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
    Assert(0);
    // ?
  }

  // Create a 6 * 52 grid of letters. There are 32 'M's randomly distributed.
  // The participant should click on as many of the Ms as he/she can in 3 mins.
  // NB How do we deal with wrong clicks, etc ?

  const int MAX_LETTERS = 6 * 52;
  const int NUM_M = 32; // from Malec et al
  char letters[MAX_LETTERS + 1];
  char no_m[26] = "ABCDEFGHIJKLNOPQRSTUVWXYZ"; // no "m"

  for (int i = 0; i < NUM_M; i++)
  {
    letters[i] = 'M';
  }

  for (int i = NUM_M; i < MAX_LETTERS; i++)
  {
    letters[i] = no_m[rand() % 25];
  }
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

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  TheEventPoller::Instance()->AddListener(m_listener);
}

void GSLetterCancellation1::OnDeactive()
{
  GSGui::OnDeactive();
  TheEventPoller::Instance()->RemoveListener(m_listener);
}

} // namespace
