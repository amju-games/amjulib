#include <Game.h>
#include "GSVe3GetObjectStates.h"
#include "GSVe3HomePage.h"
#include "ObjectUpdater.h"
#include "LocalPlayer.h"
#include "MsgManager.h"

namespace Amju
{
GSVe3GetObjectStates::GSVe3GetObjectStates()
{
}

void GSVe3GetObjectStates::Update()
{
  GSGui::Update();
  TheObjectUpdater::Instance()->Update();

  // Check if local player has name set
  Player* p = GetLocalPlayer();
  if (p)
  {
    std::string playerName = p->GetName();
    if (!playerName.empty())
    {
      std::cout << "OK, I think we can start now.\n";

      // VE3: To populate guestbook etc
      TheMsgManager::Instance()->CheckForNewMsgs();

      TheGame::Instance()->SetCurrentState(TheGSVe3HomePage::Instance());
    }
  }
}

void GSVe3GetObjectStates::Draw()
{
  GSGui::Draw();

}

void GSVe3GetObjectStates::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3GetObjectStates::OnActive()
{
  GSGui::OnActive();

  std::cout << "Checking for UPDATES now...\n";
  TheObjectUpdater::Instance()->HintCheckForUpdates();

  m_gui = LoadGui("gui-ve3-getobjectstates.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
