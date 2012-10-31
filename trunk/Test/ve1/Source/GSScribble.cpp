#include "GSScribble.h"
#include <AmjuGL.h>

namespace Amju
{
GSScribble::GSScribble()
{
}

void GSScribble::Update()
{
  GSGui::Update();

}

void GSScribble::Draw()
{
  GSGui::Draw();

}

void GSScribble::Draw2d()
{
  GSGui::Draw2d();
}

void GSScribble::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
