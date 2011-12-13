#include "GSLoginWaiting.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSLogin.h"

namespace Amju
{
static void OnCancelButton()
{
  TheGame::Instance()->SetCurrentState(TheGSLogin::Instance());
}

GSLoginWaiting::GSLoginWaiting()
{
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
}

bool GSLoginWaiting::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSLoginWaiting::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
