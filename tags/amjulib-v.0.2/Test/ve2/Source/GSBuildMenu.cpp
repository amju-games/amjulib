#include <AmjuFirst.h>
#include "GSBuildMenu.h"
#include <AmjuGL.h>
#include <AmjuFinal.h>

namespace Amju
{
GSBuildMenu::GSBuildMenu()
{
}

void GSBuildMenu::Update()
{
  //GSBase::Update();
}

void GSBuildMenu::Draw()
{
  //GSBase::Draw();
}

void GSBuildMenu::Draw2d()
{
}

void GSBuildMenu::OnActive()
{
  GSMain::OnActive();
}

bool GSBuildMenu::OnCursorEvent(const CursorEvent& ce)
{
  return false;
}

bool GSBuildMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return false;
}
} // namespace
