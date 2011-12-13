#include "GSLogin.h"
#include <AmjuGL.h>
#include "GSMain.h"
#include <Game.h>
#include "GSLoginWaiting.h"
#include <GuiTextEdit.h>

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
  GuiTextEdit* elem = dynamic_cast<GuiTextEdit*>(m_gui->GetElementByName("email"));
  Assert(elem);
  if (elem)
  {
    std::string email = elem->GetText();
    // TODO password; use https

    // Get the next state to fire off the request. This avoids the race condition where the request
    //  could complete before we load up the next state.
    TheGSLoginWaiting::Instance()->SetEmail(email);
    TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
    // Oh no, race condition, wait till state changed 
    //SendLoginReq(email);
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
