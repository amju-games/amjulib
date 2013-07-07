#include <AmjuFirst.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <EventPoller.h>
#include <Timer.h>
#include "GSCogTestBase.h"
#include "CogTestNag.h"
#include "HeartCount.h"
#include "GameConsts.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnStopTest()
{
  OnCogTestStopPartWayThrough(); 
}

GSCogTestBase::GSCogTestBase()
{
  m_isFinished = false;
  m_testId = -1;
  m_timer = 0;
  m_maxTime = 0;
  m_correct = 0;
  m_incorrect = 0;
}

void GSCogTestBase::OnActive()
{
  // Skip GSGui::OnActive
  GSBase::OnActive();
  LoadCogTestBg();
  m_isFinished = false;
}

void GSCogTestBase::UpdateCorrectIncorrect()
{
  GuiText* scoreText = (GuiText*)GetElementByName(m_gui, "correct-incorrect-text");
  std::string s = "Correct: " + ToString(m_correct) + " Incorrect: " + ToString(m_incorrect);
  scoreText->SetText(s);
}

void GSCogTestBase::UpdateScore()
{
  // Call when test complete
  ChangePlayerCount(SCORE_KEY, m_correct); // ? 
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
  // Load common gui file, then create composite of this and the 
  //  state-specific gui.
  static EventPoller* ep = TheEventPoller::Instance();
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

  GuiButton* done = (GuiButton*)GetElementByName(m_gui, "done-button");
  done->SetCommand(Amju::OnStopTest);
  done->SetVisible(true);
}
}


