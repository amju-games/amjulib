#include "GSOptions.h"
#include <AmjuGL.h>
#include <Game.h>
#include "GSAvatarMod.h"

namespace Amju
{
static void OnAvatarButton()
{
  TheGSAvatarMod::Instance()->SetPrevState(TheGSOptions::Instance());

  TheGame::Instance()->SetCurrentState(TheGSAvatarMod::Instance());
}

static void OnBack()
{
  TheGSOptions::Instance()->GoBack();
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
  m_gui->GetElementByName("back-button")->SetCommand(OnBack);
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
