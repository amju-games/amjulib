#include "GSLogin.h"
#include <AmjuGL.h>

namespace Amju
{
GSLogin::GSLogin()
{
  //m_nextState=...
}

void GSLogin::Update()
{
  GSBase::Update();

}

void GSLogin::Draw()
{
  GSBase::Draw();

}

void GSLogin::Draw2d()
{
}

void GSLogin::OnActive()
{
  GSBase::OnActive();

  m_gui = LoadGui("gui-login.txt");
  Assert(m_gui);
/*
  m_gui->GetElementByName("start-button-1p")->SetCommand(new CommandStartGame1p);
  m_gui->GetElementByName("start-button-2p")->SetCommand(new CommandStartGame2p);
  m_gui->GetElementByName("editor-button")->SetCommand(new CommandEditor);
*/
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
