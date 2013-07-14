#include "GSCogTestHelp.h"
#include <AmjuGL.h>

namespace Amju
{
GSCogTestHelp::GSCogTestHelp()
{
}

void GSCogTestHelp::Update()
{
  GSGui::Update();

}

void GSCogTestHelp::Draw()
{
  GSGui::Draw();

}

void GSCogTestHelp::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogTestHelp::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
