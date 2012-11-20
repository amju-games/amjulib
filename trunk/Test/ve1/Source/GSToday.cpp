#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include <AmjuTime.h>
#include "GSToday.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"
#include "GSTitle.h"
#include "GSStartGame.h"
#include "GSLogout.h"

namespace Amju
{
static void OnTodayPlay()
{
  TheGame::Instance()->SetCurrentState(TheGSStartGame::Instance());
}

static void OnTodayOk()
{
  GameState* state = 0;

  if (GetGameMode() == AMJU_MODE_NO_GAME)
  {
    // If no game day and it's not a research session, there is nothing to do.
    // If no game day, go straight to cog test.
    if (DoCogTests())
    {
      state = TheGSCogTestMenu::Instance();
    }
    else
    {
      // TODO We get here but never leave this state...??? WTF ???

      // Explain state ?? No, the text in this state explains.
      TheGSLogout::Instance()->SetPrevState(TheGSTitle::Instance());
      TheGame::Instance()->SetCurrentState(TheGSLogout::Instance());
    }
  }
  else if (GetGameMode() == AMJU_MODE_MULTI)
  {
    // If it's a game day, go to main state.
    state = TheGSStartGame::Instance();
  } 
  else
  {
std::cout << "Unexpected game mode!!\n";
    Assert(0);
  }
  TheGame::Instance()->SetCurrentState(state);
}

GSToday::GSToday()
{
}

void GSToday::Update()
{
  GSGui::Update();

}

void GSToday::Draw()
{
  GSGui::Draw();

}

void GSToday::Draw2d()
{
  GSGui::Draw2d();
}

void GSToday::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-today.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnTodayOk);

  // For admin/dev, button to play game even on non-game days
  GuiButton* play = (GuiButton*)GetElementByName(m_gui, "play-button");
  play->SetCommand(OnTodayPlay);
#ifndef _DEBUG
  play->SetVisible(false);
#endif

  GuiText* dateText = (GuiText*)GetElementByName(m_gui, "date-text");
  Time now = Time::Now();
  dateText->SetText(now.ToString()); // TODO just the date part

  GuiText* text = (GuiText*)GetElementByName(m_gui, "long-text");
  std::string str;
  // The cases are:
  // No game mode, and we must do test
  // No game mode, and no need to do test
  // Multi-player mode. May need to do test, will be prompted in game.
  if (GetGameMode() == AMJU_MODE_NO_GAME)
  {
    // If no game day and test required, go straight to cog test.
    if (DoCogTests())
    {
      str = "Today, please take a few tests. This should take about ten minutes. After that, you are done!";
      // TODO Get next date from the ResearchCalendar -- show in final thanks page
    }
    else
    {
      // If no game day and it's not a research session, there is nothing to do.
      str = "There is nothing for you to do today. Thank you for participating!";
      // TODO Get next date from the ResearchCalendar
    }
  }
  else if (GetGameMode() == AMJU_MODE_MULTI)
  {
    // If it's a game day, go to main state.
    str = "Please play the game for as long as you like, up to one hour. Please feel free to interact with other players as much as you like.";
    if (DoCogTests())
    {
      str += "\n\nYou will be asked to take a few tests during the game. Thanks for your help!";
    }
  } 
  else
  {
std::cout << "Unexpected game mode!!\n";
    Assert(0);
  }
  text->SetText(str);
}

} // namespace
