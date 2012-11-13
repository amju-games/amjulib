#include "GSAchievements.h"
#include <AmjuGL.h>

namespace Amju
{
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

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
