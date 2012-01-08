#include "GSChoosePlayer.h"
#include <AmjuGL.h>
#include <StringUtils.h>
#include "PlayerInfo.h"
#include <GuiButton.h>
#include <Game.h>
#include "GSTitle.h"
#include "GSLogin.h"
#include "GSLoginWaiting.h"

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
  ChoosePlayerCommand(int playerNum, const std::string& email) : m_playerNum(playerNum), m_email(email) {}

  virtual bool Do()
  {
    TheGSLoginWaiting::Instance()->SetEmail(m_email);
    TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());

    return false;
  }

private:
  int m_playerNum;
  std::string m_email;
};

void GSChoosePlayer::OnActive()
{
  Strings players = ThePlayerInfoManager::Instance()->GetPlayerNames();
  int numPlayers = players.size();

  if (numPlayers == 0)
  {
    OnNewPlayer();
    return;
  }

  GSGui::OnActive();

  m_gui = LoadGui("gui-chooseplayer.txt");
  Assert(m_gui);

  m_gui->GetElementByName("cancel-button")->SetCommand(OnCancelButton);

std::cout << "Num players: " << numPlayers << "\n";

  for (int i = 0; i < 4; i++)
  {
    GuiElement* elem = m_gui->GetElementByName("p" + ToString(i) + "-button");
    GuiButton* b = dynamic_cast<GuiButton*>(elem);
    Assert(b);

    if (i < numPlayers)
    {
      ThePlayerInfoManager::Instance()->SetCurrentPlayer(players[i]);
      PlayerInfo* pi = ThePlayerInfoManager::Instance()->GetPI();
      Assert(pi);
      std::string playername = pi->PIGetString(PI_KEY("playername"));
      std::string email = pi->PIGetString(PI_KEY("email"));

      b->SetText(playername);
      b->SetCommand(new ChoosePlayerCommand(i, email));
    }
    else if (i == numPlayers)
    {
      b->SetText("new...");
      b->SetCommand(OnNewPlayer);
    }
    else
    {
      elem->SetVisible(false);
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
