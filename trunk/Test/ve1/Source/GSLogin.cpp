#include "GSLogin.h"
#include <AmjuGL.h>
#include "GSMain.h"
#include <Game.h>
#include "GSLoginWaiting.h"
#include <GuiTextEdit.h>
#include "ReqLogin.h"

namespace Amju
{
void OnLoginButton()
{
  TheGSLogin::Instance()->OnLoginButton();
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

  m_gui->GetElementByName("login-button")->SetCommand(Amju::OnLoginButton);

//  m_gui->GetElementByName("start-button-2p")->SetCommand(new CommandStartGame2p);
//  m_gui->GetElementByName("editor-button")->SetCommand(new CommandEditor);

}

void GSLogin::OnLoginButton()
{
  GuiElement* elem = m_gui->GetElementByName("email");
  if (elem && dynamic_cast<GuiTextEdit*>(elem))
  {
    std::string email = ((GuiTextEdit*)elem)->GetText();
    // TODO password; use https

    SendLoginReq(email);

    TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
  }
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
