#include <AmjuFirst.h>
#include "GSChoosePlayer.h"
#include <AmjuGL.h>
#include <StringUtils.h>
#include "PlayerInfo.h"
#include <GuiButton.h>
#include <Game.h>
#include "GSTitle.h"
#include "GSLogin.h"
#include "GSLoginWaiting.h"
#include "GameMode.h"
#include "GSMain.h"
#include "LocalPlayer.h"
#include "GSStartGame.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnNewPlayer()
{
  TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
}

static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

GSChoosePlayer::GSChoosePlayer()
{
}

void GSChoosePlayer::Update()
{
  GSGui::Update();

}

void GSChoosePlayer::Draw()
{
  GSGui::Draw();

}

void GSChoosePlayer::Draw2d()
{
  GSGui::Draw2d();
}

class ChoosePlayerCommand : public GuiCommand
{
public:
  ChoosePlayerCommand(int playerNum, const std::string& playername) :
    m_playerNum(playerNum), m_playername(playername) { }

  virtual bool Do()
  {
    ThePlayerInfoManager::Instance()->SetCurrentPlayer(m_playername);
    PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
    Assert(pi);
    std::string email = pi->PIGetString(PI_KEY("email"));
    TheGSLoginWaiting::Instance()->SetEmail(email);
    TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());

    return false;
  }

private:
  int m_playerNum;
  std::string m_playername;
};

void GSChoosePlayer::OnActive()
{
  GSGui::OnActive();

  Strings players = ThePlayerInfoManager::Instance()->GetPlayerNames();
  int numPlayers = players.size();

  if (numPlayers == 0)
  {
std::cout << "GSChoosePlayer: no existing player names.\n";

    OnNewPlayer();
    return;
  }

  m_gui = LoadGui("gui-chooseplayer.txt");
  Assert(m_gui);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(OnCancelButton);
  cancel->SetIsCancelButton(true);

std::cout << "GSChoosePlayer: Num players: " << numPlayers << "\n";

  static int choice = 0;
  for (int i = 0; i < 4; i++)
  {
    GuiElement* elem = m_gui->GetElementByName("p" + ToString(i) + "-button");
    GuiButton* b = dynamic_cast<GuiButton*>(elem);
    Assert(b);

    if (i == choice)
    {
      b->SetHasFocus(true);
    }

    if (i < numPlayers)
    {
      // Get player info without touching timestamp
      std::string playername = players[i]; 
      std::string buttonname = Replace(playername, ".txt", "");

      b->SetText(buttonname);
      b->SetCommand(new ChoosePlayerCommand(i, playername));
    }
    else if (i == numPlayers)
    {
      b->SetText("New...");
      b->SetCommand(OnNewPlayer);
    } 
    else
    {
      b->SetVisible(false);
    }
  }
}

bool GSChoosePlayer::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSChoosePlayer::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
