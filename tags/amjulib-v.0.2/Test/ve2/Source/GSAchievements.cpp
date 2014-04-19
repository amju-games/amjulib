#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include "GSAchievements.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnAchievementsOk()
{
  TheGSAchievements::Instance()->GoBack();
}

GSAchievements::GSAchievements()
{
}

void GSAchievements::Update()
{
  GSGui::Update();
}

void GSAchievements::Draw()
{
  GSGui::Draw();
}

void GSAchievements::Draw2d()
{
  GSGui::Draw2d();
}

void GSAchievements::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-achievements.txt");
  Assert(m_gui);

  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnAchievementsOk);
}

} // namespace
