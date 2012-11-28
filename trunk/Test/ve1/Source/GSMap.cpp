#include "GSMap.h"
#include <AmjuGL.h>

namespace Amju
{
GSMap::GSMap()
{
}

void GSMap::Update()
{
  GSGui::Update();

}

void GSMap::Draw()
{
  GSGui::Draw();

}

void GSMap::Draw2d()
{
  GSGui::Draw2d();
}

void GSMap::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
