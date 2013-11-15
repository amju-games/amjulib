#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <StringsFile.h>
#include <GuiButton.h>
#include <GuiText.h>
#include "GSStory.h"
#include "LocalPlayer.h"
#include <AmjuFinal.h>

namespace Amju
{
static const char* STORY_NUM_KEY = "story_num";

void OnStoryReadOk()
{
  TheGSStory::Instance()->OnReadOk();
}

void OnStoryDone()
{
  TheGSStory::Instance()->GoBack(); 
}

GSStory::GSStory()
{
  m_showedOk = false;
  m_storyLineNum = 0;
}

void GSStory::OnReadOk()
{
  m_showedOk = true;
  GoBack();
}

void GSStory::Update()
{
  GSGui::Update();
}

void GSStory::Draw()
{
  GSGui::Draw();
}

void GSStory::Draw2d()
{
  GSGui::Draw2d();
}

void GSStory::SetText(const std::string& text)
{
  m_text = text;
}

void GSStory::OnActive()
{
  GSGui::OnActive();

  // Play storyline music
  // TODO

  m_gui = LoadGui("gui-story.txt");
  Assert(m_gui);

  // Load story text
  Strings storyLines;

  static bool first = true;
  if (first)
  {
    if (!LoadStrings("story.txt", &storyLines))
    {
#ifdef _DEBUG
std::cout << "Can't show story, FAILED TO LOAD STORY FILE!!\n";
#endif
      OnStoryDone();
      return;
    }
  
    first = false;
  }

  // Set text. This depends where we are in the story. It could be a tree, but assume linear for now.
  // Need to be logged in for this to work:
  Player* p = GetLocalPlayer();
  if (!p)
  {
#ifdef _DEBUG
std::cout << "Can't show story, no player is logged in.\n";
#endif

    OnStoryDone();
    return;
  }
  Assert(p);
  std::string storyNum = "0";
  if (p->Exists(STORY_NUM_KEY))
  {
    storyNum = p->GetVal(STORY_NUM_KEY);
    m_storyLineNum = ToInt(storyNum);
  }

  m_showedOk = false; 
  // TODO Set focus element, cancel element, command handlers

  GuiText* text = (GuiText*)GetElementByName(m_gui, "story-text");
  Assert(text);
  if (m_text.empty())
  {
    text->SetText(storyLines[m_storyLineNum]);
  }
  else
  {
    text->SetText(m_text); 
  }

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "story-ok-button");
  Assert(ok);
  // TODO Set couple of seconds after last page of story fragment has been shown.
  // (TODO multi page - just single page for now.)
  ok->SetCommand(OnStoryReadOk); 
}

void GSStory::OnDeactive()
{
  Player* p = GetLocalPlayer();
  if (p && m_showedOk)
  {
    // Go to next story for next time -- do this is OnDeactivated ?
    std::string storyNum = ToString(m_storyLineNum + 1);
    p->SetKeyVal(STORY_NUM_KEY, storyNum);
  }
}
} // namespace

