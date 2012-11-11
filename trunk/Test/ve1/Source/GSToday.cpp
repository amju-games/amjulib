#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include "GSToday.h"
#include "GameMode.h"
#include "GSCogTestMenu.h"
#include "GSTitle.h"
#include "GSStartGame.h"

namespace Amju
{
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
      // Explain state ?? No, the text in this state explains.
      state = TheGSTitle::Instance();
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

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnTodayOk);
}

} // namespace
