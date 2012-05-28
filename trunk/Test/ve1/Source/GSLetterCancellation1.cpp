#include "GSLetterCancellation1.h"
#include <AmjuGL.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <ResourceManager.h>
#include <EventPoller.h>
#include <Game.h>
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
}

bool GSLetterCancellation1::OnCursorEvent(const CursorEvent& ce)
{
  unsigned int s = m_letters.size();
  for (unsigned int i = 0; i < s; i++)
  {
    GuiElement* elem = m_letters[i];
    Rect r = GetRect(elem);
    // TODO highlight any selected letter    
    if (r.IsPointIn(Vec2f(ce.x, ce.y)))
    {
      GuiText* text = (GuiText*)elem;
      text->SetInverse(false);
      break;
    }
  }
  return false;
}

bool GSLetterCancellation1::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}

void GSLetterCancellation1::Update()
{
  GSGui::Update();
 
}

void GSLetterCancellation1::Draw()
{
  GSGui::Draw();

}

void GSLetterCancellation1::Draw2d()
{
  GSGui::Draw2d();
}

void GSLetterCancellation1::OnActive()
{
  // Reset timer and score

  GSGui::OnActive();

  m_gui = LoadGui("gui-lettercancellation.txt");
  Assert(m_gui);
  GuiComposite* comp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  Assert(comp);

  // Create a 6 * 52 grid of letters. There are 32 'M's randomly distributed.
  // The participant should click on as many of the Ms as he/she can in 3 mins.
  // NB How do we deal with wrong clicks, etc ?

  // Add letters - grid of 6 rows with 52 letters in each. This is unlikely to change!
  m_letters.clear();
  m_letters.reserve(312);

  const int MAX_LETTERS = 312;
  const int NUM_M = 32; // from Malec et al
  char letters[MAX_LETTERS + 1];
  char no_m[26] = "abcdefghijklnopqrstuvwxyz"; // no "m"

  for (int i = 0; i < NUM_M; i++)
  {
    letters[i] = 'm';
  }

  for (int i = NUM_M; i < MAX_LETTERS; i++)
  {
    letters[i] = no_m[rand() % 25];
  }
  std::random_shuffle(letters, letters + MAX_LETTERS);

  static const float TOP = 0.5f; // TODO CONFIG
  static const float LEFT = -1.0f; // TODO CONFIG
  static const float V_SPACE = 0.15f;
  static const float H_SPACE = 0.05f;

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 52; j++)
    {
#ifdef USE_BUTTONS_NOT_TEXT
      static Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes("wh8.png");

      GuiButton* button = new GuiButton;
      button->SetTexture(tex);
      button->SetTextColour(Colour(0, 0, 0, 1));
      // etc
      comp->AddChild(button);

#else
      GuiText* text = new GuiText;
      text->SetText(std::string(1, letters[i * 52 + j]));
      text->SetTextSize(1.0f); // TODO Broken for < 1.0!!!
      text->SizeToText();
      //text->SetSize(Vec2f(0.1f, 0.1f)); // TODO 
      text->SetDrawBg(true);
      text->SetInverse(true);
      text->SetJust(GuiText::AMJU_JUST_LEFT);
      text->SetLocalPos(Vec2f(LEFT + (float)j * H_SPACE, TOP - (float)i * V_SPACE));
      comp->AddChild(text);
      m_letters.push_back(text);

#endif
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
