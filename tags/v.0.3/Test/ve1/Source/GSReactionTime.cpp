#include "GSReactionTime.h"
#include <AmjuGL.h>
#include <ReportError.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <GuiComposite.h>
#include <Game.h>
#include <DrawRect.h>
#include <Timer.h>
#include "GSCogTestMenu.h"
#include "CogTestResults.h"

namespace Amju
{
static void OnButton()
{
  TheGSReactionTime::Instance()->OnButton();
}

GSReactionTime::GSReactionTime()
{
  m_reactionTime = 0; 
  m_waitTime = 0; 
  m_maxWaitTime = 0; 
  m_testNum = 0; 
  m_maxTestNum = 3;
  m_mode = RT_BEFORE_TEST;
}

void GSReactionTime::OnButton()
{
  switch (m_mode)
  {
  case RT_TIMING:
    {
std::cout << "Reaction time result: " << m_reactionTime << "s\n";

    // Send results
    TheCogTestResults::Instance()->StoreResult(new Result(
      AMJU_COG_TEST_REACTION_TIME, "time" + ToString(m_testNum), ToString(m_reactionTime)));

    m_mode = RT_AFTER_TEST;
    GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
    b->SetIsEnabled(false); 
    b->SetText("");
      
    GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
    word->SetText(ToString(m_reactionTime, 2));
    }
    break;

  case RT_CONTINUE:
    m_testNum++;
    if (m_testNum > m_maxTestNum)
    {
      TheGame::Instance()->SetCurrentState(TheGSCogTestMenu::Instance());
    }
    else
    {
      SetTest();
    }
    break;

  default:
    Assert(0);
  }
}

void GSReactionTime::Update()
{
  GSGui::Update();

  float dt = TheTimer::Instance()->GetDt();

  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  GuiText* timeText = (GuiText*)GetElementByName(m_gui, "timer");

  switch (m_mode)
  {
  case RT_BEFORE_TEST:
    m_waitTime += dt;
    if (m_waitTime >= 3.0f) // TODO TEMP TEST
    {
      m_waitTime = 0;
      word->SetText("");
      m_mode = RT_WAITING;
    }
    break;

  case RT_WAITING:
    m_waitTime += dt;
    if (m_waitTime >= m_maxWaitTime)
    {
      m_waitTime = 0;
      word->SetText("GO!");
      b->SetText("click me!");
      m_mode = RT_TIMING;
      b->SetIsEnabled(true); 
    }
    break;

  case RT_AFTER_TEST:
    m_waitTime += dt;
    if (m_waitTime >= 3.0f) // TODO TEMP TEST
    {
      m_mode = RT_CONTINUE;
      b->SetText("next...");
      b->SetIsEnabled(true);
    }
    break;

  case RT_TIMING:
    m_reactionTime += dt;
    timeText->SetText(ToString(m_reactionTime, 2));
    break;

  case RT_CONTINUE:
    // Wait for button press
    break;
  }
}

void GSReactionTime::Draw()
{
//  GSGui::Draw();

}

void GSReactionTime::Draw2d()
{
  GSGui::Draw2d();

  switch (m_mode)
  {
  case RT_BEFORE_TEST:
    break;

  case RT_WAITING:
    break;

  case RT_TIMING:
    break;

  case RT_AFTER_TEST:
    // Show time, etc
    break;

  case RT_CONTINUE:
    break;   
  }
}

void GSReactionTime::OnActive()
{
//  GSGui::OnActive();

  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-reactiontime.txt");
  Assert(m_gui);

  // Set focus element, cancel element, command handlers
  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
  b->SetCommand(Amju::OnButton);
  b->SetHasFocus(true);

  m_testNum = 0; 
  SetTest();
}

void GSReactionTime::SetTest()
{
std::cout << "Reaction time, test number: " << m_testNum << "\n";
  GuiButton* b = (GuiButton*)GetElementByName(m_gui, "button");
  b->SetIsEnabled(false); 
  b->SetText("");

  GuiText* word = (GuiText*)GetElementByName(m_gui, "word");
  word->SetText("ready...");

  m_maxWaitTime = 4.0f + (float)rand() / (float)RAND_MAX * 3.0f; // TODO Rand between ? and 7.0
std::cout << "Waiting time is: " << m_maxWaitTime << "s\n";

  m_mode = RT_BEFORE_TEST;
  m_waitTime = 0;
  m_reactionTime = 0;
}

} // namespace
