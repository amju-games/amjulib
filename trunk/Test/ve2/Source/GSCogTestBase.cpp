#include <GuiButton.h>
#include <GuiComposite.h>
#include <EventPoller.h>
#include <Timer.h>
#include "GSCogTestBase.h"
#include "CogTestNag.h"

namespace Amju
{
static void OnStopTest()
{
  OnCogTestStopPartWayThrough(); 
}

GSCogTestBase::GSCogTestBase()
{
  m_isFinished = false;
}

void GSCogTestBase::OnActive()
{
  // Skip GSGui::OnActive
  GSBase::OnActive();
  LoadCogTestBg();
  m_isFinished = false;
}

void GSCogTestBase::UpdateTimer()
{
  m_timer -= TheTimer::Instance()->GetDt();

  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");
  std::string s;
  if (!m_isFinished)
  {
    if (m_timer > 0)
    {
      int min = (int)(m_timer / 60.0f);
      int sec = (int)(m_timer - 60.0f * min);
      s = ToString(min) + ":" + (sec < 10 ? "0" : "") + ToString(sec);
    }
    else
    {
      // TODO flash
      s = "0:00";

      Finished();
    }

    timeText->SetText(s);
  }
}

void GSCogTestBase::LoadCommonGui()
{
#ifdef WTF
  // Load common gui file, then create composite of this and the 
  //  state-specific gui.
  EventPoller* ep = TheEventPoller::Instance();
  if (ep->HasListener(m_gui))
  {
    ep->RemoveListener(m_gui);
  }

  PGuiElement common = LoadGui("gui-cog-test-common.txt");
  Assert(common);
  if (ep->HasListener(common))
  {
    ep->RemoveListener(common);
  }

  GuiComposite* comp = new GuiComposite;
  comp->AddChild(m_gui);
  comp->AddChild(common);
  m_gui = comp;
  ep->AddListener(m_gui);

  PGuiElement common = LoadGui("gui-cog-test-common.txt");
  Assert(common);
  static EventPoller* ep = TheEventPoller::Instance();
  if (ep->HasListener(common))
  {
    ep->RemoveListener(common);
  }
  GuiComposite* comp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  if (comp)
  {
    comp->AddChild(common);
  }

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnStopTest);
  done->SetVisible(true);
#endif

}
}


