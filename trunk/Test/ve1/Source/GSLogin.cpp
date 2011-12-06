#include "GSLogin.h"
#include <AmjuGL.h>
#include "GSMain.h"
#include <Game.h>

namespace Amju
{
void OnLoginPressed()
{
  TheGame::Instance()->SetCurrentState(TheGSMain::Instance());
}


GSLogin::GSLogin()
{
}

void GSLogin::Update()
{
  GSGui::Update();

}

void GSLogin::Draw()
{
  GSGui::Draw();

}

void GSLogin::Draw2d()
{
  GSGui::Draw2d();
}

void GSLogin::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-login.txt");
  Assert(m_gui);

  m_gui->GetElementByName("login-button")->SetCommand(OnLoginPressed);

//  m_gui->GetElementByName("start-button-2p")->SetCommand(new CommandStartGame2p);
//  m_gui->GetElementByName("editor-button")->SetCommand(new CommandEditor);

}

bool GSLogin::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLogin::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
