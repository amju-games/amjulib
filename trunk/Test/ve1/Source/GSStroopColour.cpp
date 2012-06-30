#include "GSStroopColour.h"
#include <AmjuGL.h>

namespace Amju
{
GSStroopColour::GSStroopColour()
{
}

void GSStroopColour::Update()
{
  GSGui::Update();

}

void GSStroopColour::Draw()
{
  GSGui::Draw();

}

void GSStroopColour::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopColour::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
