#include <AmjuFirst.h>
#include "GSLoginWaiting.h"
#include <AmjuGL.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiText.h>
#include "GSTitle.h"
#include "ReqLogin.h"
#include <AmjuFinal.h>

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
  if (m_time > 10.0f) // TODO CONFIG
  {
    GuiButton* cancel = (GuiButton*)m_gui->GetElementByName("cancel-button");
    cancel->SetVisible(true);
  }
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

  GuiButton* cancel = (GuiButton*)m_gui->GetElementByName("cancel-button");
  cancel->SetCommand(Amju::OnCancelButton);
  //cancel->SetVisible(false); // make visible after delay
  cancel->SetIsCancelButton(true);

  // Now it's safe to create the login request -- however quickly it completes, we are ready!
  SendLoginReq(m_email);
}

} // namespace
