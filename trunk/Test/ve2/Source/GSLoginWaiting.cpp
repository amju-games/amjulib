#include "GSLoginWaiting.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSTitle.h"
#include <GuiText.h>
#include "ReqLogin.h"

namespace Amju
{
static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

GSLoginWaiting::GSLoginWaiting()
{
}

void GSLoginWaiting::SetErrorString(const std::string& error)
{
  Assert(m_gui);

  GuiText* t = dynamic_cast<GuiText*>(m_gui->GetElementByName("error-string"));
  Assert(t);
  if (t)
  {
    t->SetText(error);
  }
}

void GSLoginWaiting::Update()
{
  GSGui::Update();

}

void GSLoginWaiting::Draw()
{
  GSGui::Draw();

}

void GSLoginWaiting::Draw2d()
{
  GSGui::Draw2d();
}

void GSLoginWaiting::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-login-wait.txt");
  Assert(m_gui);

  m_gui->GetElementByName("cancel-button")->SetCommand(Amju::OnCancelButton);

  // Now it's safe to create the login request -- however quickly it completes, we are ready!
  SendLoginReq(m_email);
}

} // namespace
