#include <GuiButton.h>
#include "GSLogin.h"
#include <AmjuGL.h>
#include "GSMain.h"
#include <Game.h>
#include <GuiTextEdit.h>
#include "GSLoginWaiting.h"
#include "GSTitle.h"
#include <ConfigFile.h>

namespace Amju
{
static std::string lastEmail = "person@example.com";

static void OnLoginButton()
{
  TheGSLogin::Instance()->OnLoginButton();
}

static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
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

  GuiButton* login = (GuiButton*)GetElementByName(m_gui, "login-button");
  login->SetCommand(Amju::OnLoginButton);
  login->SetHasFocus(true);

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  GetElementByName(m_gui, "email")->SetHasFocus(true);

  // TODO Set last email address entered ?
  // TODO use config ?
  //static GameConfigFile* config = TheGameConfigFile::Instance();
  GuiTextEdit* elem = dynamic_cast<GuiTextEdit*>(m_gui->GetElementByName("email"));
  Assert(elem);
  elem->SetText(lastEmail);
}

void GSLogin::OnLoginButton()
{
  GuiTextEdit* elem = dynamic_cast<GuiTextEdit*>(m_gui->GetElementByName("email"));
  Assert(elem);
  if (elem)
  {
    std::string email = elem->GetText();
    lastEmail = email;
    // TODO Save email in prefs

    // TODO password; use https

    // Get the next state to fire off the request. This avoids the race condition where the request
    //  could complete before we load up the next state.
    TheGSLoginWaiting::Instance()->SetEmail(email);
    TheGame::Instance()->SetCurrentState(TheGSLoginWaiting::Instance());
    // Oh no, race condition, wait till state changed 
    //SendLoginReq(email);
  }
}

} // namespace
