#include "GSTitle.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSLogin.h"

namespace Amju
{
static void OnStartButton()
{
  TheGame::Instance()->SetCurrentState(TheGSLogin::Instance()); 
}

GSTitle::GSTitle()
{
}

void GSTitle::Update()
{
  GSGui::Update();

}

void GSTitle::Draw()
{
  GSGui::Draw();

}

void GSTitle::Draw2d()
{
  GSGui::Draw2d();
}

void GSTitle::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-title.txt");
  Assert(m_gui);

  m_gui->GetElementByName("start-button")->SetCommand(Amju::OnStartButton);
}

bool GSTitle::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSTitle::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
