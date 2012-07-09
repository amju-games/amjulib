#include <GuiButton.h>
#include <AmjuGL.h>
#include <Game.h>
#include <GuiTextEdit.h>
#include <ConfigFile.h>
#include "GSLogin.h"
#include "GSLoginWaiting.h"
#include "GSTitle.h"
#include "Kb.h"

namespace Amju
{
static std::string lastEmail = "";

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

  static Kb* kb = TheKb::Instance();
  kb->Update();
}

void GSLogin::Draw()
{
  GSGui::Draw();

}

void GSLogin::Draw2d()
{
  static Kb* kb = TheKb::Instance();
  kb->Draw();

  GSGui::Draw2d();
}

void OnLoginChar()
{
  TheGSLogin::Instance()->OnLoginChar();
}

void GSLogin::OnLoginChar()
{
  GuiTextEdit* text = (GuiTextEdit*)(m_gui->GetElementByName("email"));
  std::string s = text->GetText();
  bool b = (s.find('@') != std::string::npos && s.find('.') != std::string::npos);
  GuiButton* login = (GuiButton*)GetElementByName(m_gui, "login-button");
  if (b)
  {
std::cout << "Email is ok, enable login button.\n";
    // Set button to be focus, then text 
    login->SetIsEnabled(true);
    login->SetHasFocus(true); 
    text->SetHasFocus(true); 
  }
  else
  {
std::cout << "Email is NOT ok, disable login button.\n";
    login->SetHasFocus(false); 
    login->SetIsEnabled(false); 
  }
}

void GSLogin::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-login.txt");
  Assert(m_gui);

  GuiButton* login = (GuiButton*)GetElementByName(m_gui, "login-button");
  login->SetCommand(Amju::OnLoginButton);
  login->SetHasFocus(true);
  login->SetIsEnabled(false); // Until we type a valid email

  GuiButton* cancel = (GuiButton*)GetElementByName(m_gui, "cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  cancel->SetIsCancelButton(true);

  GetElementByName(m_gui, "email")->SetHasFocus(true);

  // TODO Set last email address entered ?
  // TODO use config ?
  //static GameConfigFile* config = TheGameConfigFile::Instance();
  GuiTextEdit* text = (GuiTextEdit*)(m_gui->GetElementByName("email"));
  text->SetText(lastEmail);
  text->SetOnChangeFunc(Amju::OnLoginChar);

  static Kb* kb = TheKb::Instance();
  if (kb->IsEnabled())
  {
    kb->Activate();
    // Set floor of this GUI
    Vec2f pos = m_gui->GetLocalPos();
    pos.y = 0.7f; // TODO depends on gui txt file, ideally we set this to below the bottom element.
    m_gui->SetLocalPos(pos);
  }
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
  
    TheKb::Instance()->Deactivate();
  }
}

} // namespace
