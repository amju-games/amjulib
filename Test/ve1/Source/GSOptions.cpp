#include "GSOptions.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSAvatarMod.h"

namespace Amju
{
void OnAvatarButton()
{
  TheGSAvatarMod::Instance()->SetPrevState(TheGSOptions::Instance());

  TheGame::Instance()->SetCurrentState(TheGSAvatarMod::Instance());
}

GSOptions::GSOptions()
{
}

void GSOptions::Update()
{
  GSGui::Update();

}

void GSOptions::Draw()
{
  GSGui::Draw();

}

void GSOptions::Draw2d()
{
  GSGui::Draw2d();
}

void GSOptions::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-options.txt");
  Assert(m_gui);
  m_gui->GetElementByName("avatar-button")->SetCommand(OnAvatarButton);
}

bool GSOptions::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSOptions::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
