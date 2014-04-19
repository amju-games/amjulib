#include <Game.h>
#include "GSVe3GetObjects.h"
#include "GSVe3GetObjectStates.h"
#include "ObjectManager.h"

namespace Amju
{
GSVe3GetObjects::GSVe3GetObjects()
{
}

void GSVe3GetObjects::Update()
{
  GSGui::Update();

  TheObjectManager::Instance()->Update();

  // Check if objects have been created
  GameObjects* gos = TheObjectManager::Instance()->GetGameObjects();
  if (!gos->empty())
  {
    std::cout << "We have created a bunch of objects... now we need to get their state...\n";
    TheGame::Instance()->SetCurrentState(TheGSVe3GetObjectStates::Instance());
  }
}

void GSVe3GetObjects::Draw()
{
  GSGui::Draw();

}

void GSVe3GetObjects::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3GetObjects::OnActive()
{
  GSGui::OnActive();

  TheObjectManager::Instance()->CheckForNewObjectsNow();

  m_gui = LoadGui("gui-ve3-getobjects.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
