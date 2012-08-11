#include "GSQuitGame.h"
#include <AmjuGL.h>

namespace Amju
{
GSQuitGame::GSQuitGame()
{
}

void GSQuitGame::Update()
{
  GSGui::Update();

}

void GSQuitGame::Draw()
{
  GSGui::Draw();

}

void GSQuitGame::Draw2d()
{
  GSGui::Draw2d();
}

void GSQuitGame::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-quitgame.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
