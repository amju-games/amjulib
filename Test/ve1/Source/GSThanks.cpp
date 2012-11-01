#include "GSThanks.h"
#include <AmjuGL.h>

namespace Amju
{
GSThanks::GSThanks()
{
}

void GSThanks::Update()
{
  GSGui::Update();

}

void GSThanks::Draw()
{
  GSGui::Draw();

}

void GSThanks::Draw2d()
{
  GSGui::Draw2d();
}

void GSThanks::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
