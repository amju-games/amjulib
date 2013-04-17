#include <AmjuFirst.h>
#include <Timer.h>
#include <EventPoller.h>
#include <GuiButton.h>
#include <StringUtils.h>
#include "Kb.h"
#include "ChatConsole.h"
#include <AmjuFinal.h>

namespace Amju
{
void KbSetLayout(KbLayout layout)
{
  Strings strs;

  switch (layout)
  {
  case KB_LAYOUT_REGULAR:
    strs.push_back("gui-kb-letter-lower.txt");
    strs.push_back("gui-kb-letter-upper.txt");
    strs.push_back("gui-kb-num.txt");
    strs.push_back("gui-kb-symbol.txt");
    break;

  case KB_LAYOUT_EMAIL:
    strs.push_back("gui-kb-email-lower.txt");
    strs.push_back("gui-kb-letter-upper.txt");
    strs.push_back("gui-kb-num.txt");
    strs.push_back("gui-kb-symbol.txt");
    break;

  default:
    std::cout << "Unexpected kb layout! " << layout << "\n";
    Assert(0);
  }

  TheKb::Instance()->SetPages(strs); // loads first one
}

void OnNextPage()
{
  static Kb* kb = TheKb::Instance();
  kb->OnNextPage();
}

void OnPrevPage()
{
  static Kb* kb = TheKb::Instance();
  kb->OnPrevPage();
}

void OnShow()
{
  static Kb* kb = TheKb::Instance();
  kb->SetEnabled(true);
  kb->Activate();
}

void OnHide()
{
  static Kb* kb = TheKb::Instance();
  kb->Deactivate(); // Not SetEnabled(false), so we get the hiding animation. Right??
}

Kb::Kb()
{
  m_mode = KB_HIDDEN;

  // Decide whether to enable or not. Some platforms must have soft kb -
  //  otherwise it's a user pref.

  SetEnabled(false); 

  m_currentPage = -1;
}

bool Kb::Load(const std::string& guiKbFilename)
{
std::cout << "KB: Loading kb layout: " << guiKbFilename << "\n";

  // Remove old listener 
  Deactivate();

  m_kb = new GuiKeyboard;
  bool b = m_kb->OpenAndLoad(guiKbFilename); 
  if (!b)
  {
    return false;
  }
  // Set up handler for next page button
  GuiButton* nextpage = (GuiButton*)m_kb->GetElementByName("nextpage");
  if (nextpage)
  {
    nextpage->SetCommand(Amju::OnNextPage);
    if (m_pages.size() < 2)
    {
std::cout << "Next page set but num pages is " << m_pages.size() << "!!\n";
      Assert(0);
    }
  }
  else
  {
std::cout << "KB: no next page for this layout: " << guiKbFilename << "\n";
  }

  GuiButton* prevpage = (GuiButton*)m_kb->GetElementByName("prevpage");
  if (prevpage)
  {
    prevpage->SetCommand(Amju::OnPrevPage);
    if (m_pages.size() < 2)
    {
std::cout << "Prev page set but num pages is " << m_pages.size() << "!!\n";
      Assert(0);
    }
  }
  else
  {
std::cout << "KB: no prev page for this layout: " << guiKbFilename << "\n";
  }

  GuiButton* hide = (GuiButton*)m_kb->GetElementByName("hide");
  if (hide)
  {
    hide->SetCommand(Amju::OnHide);
  }
  else
  {
std::cout << "KB: no hide button for this layout: " << guiKbFilename << "\n";
  }

  m_showButton = new GuiButton;
  if (!m_showButton->OpenAndLoad("gui-kb-showbutton.txt"))
  {
    std::cout << "KB: failed to load show button.\n";
    return false;
  }
  m_showButton->SetCommand(Amju::OnShow);

  return true;
}

void Kb::SetPages(const Strings& filenames)
{
  m_pages = filenames;
  m_currentPage = -1;
  OnNextPage();
}

void Kb::OnNextPage()
{
  m_currentPage++;
  Assert(m_currentPage <= (int)m_pages.size());

  if (m_currentPage == (int)m_pages.size())
  {
    m_currentPage = 0;
  } 
  const std::string& page = m_pages[m_currentPage];
  bool b = Load(page);
  if (!b)
  {
std::cout << "Failed to load KB page: " << page << "\n";
  }
  m_mode = KB_SHOWING;
  Activate();
}

void Kb::OnPrevPage()
{
  m_currentPage--;
  if (m_currentPage == -1)
  {
    m_currentPage = (int)m_pages.size() - 1;
  }
  const std::string& page = m_pages[m_currentPage];
  bool b = Load(page);
  if (!b)
  {
std::cout << "Failed to load KB page: " << page << "\n";
  }
  m_mode = KB_SHOWING;
  Activate();
}

void Kb::SetEnabled(bool b)
{
  m_enabled = b;
}

bool Kb::IsEnabled() const
{
  return m_enabled;
}

void Kb::Update()
{
  if (!m_enabled) 
  {
    return;
  }

  Assert(m_kb); // call Load first

  static const float vel = 10.0f;
  float ypos = -1 + m_kb->GetSize().y;

  switch (m_mode)
  {
  case KB_SHOWN:
    m_kb->SetVisible(true);
    break;

  case KB_HIDDEN:
    break;

  case KB_SHOWING:
    {
    m_kb->SetVisible(true);
    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_kb->GetLocalPos();
    pos.y += vel * dt;
    if (pos.y > ypos)
    {
      pos.y = ypos;
      m_mode = KB_SHOWN;
    }
    m_kb->SetLocalPos(pos);
    }
    break;

  case KB_HIDING:
    {
    float dt = TheTimer::Instance()->GetDt();
    Vec2f pos = m_kb->GetLocalPos();
    pos.y -= vel * dt;
    if (pos.y < -1.0f)
    {
      m_mode = KB_HIDDEN;
      m_kb->SetVisible(false);
      SetEnabled(false);
    }
    m_kb->SetLocalPos(pos);
    }
    break;
  }
}

void Kb::Draw()
{
  if (m_enabled)
  {
    m_kb->Draw();
  }
  else
  {
    m_showButton->Draw();
  }
}

void Kb::Activate()
{
  static EventPoller* ep = TheEventPoller::Instance();
  if (!m_enabled)
  {
    ep->AddListener(m_showButton);
    return;
  }

  Assert(m_kb); // call Load first

  Vec2f pos = m_kb->GetLocalPos();
  pos.y = -1.0f;
  m_kb->SetLocalPos(pos);

  m_mode = KB_SHOWING;

  if (!ep->HasListener(m_kb))
  {
    ep->AddListener(m_kb);
  }
}

void Kb::Deactivate()
{
/*
  if (!m_enabled)
  {
    return;
  }
*/

  m_mode = KB_HIDING;
  static EventPoller* e = TheEventPoller::Instance();

  if (m_kb && e->HasListener(m_kb))
  {
    e->RemoveListener(m_kb);
  }
}

}

