#include "GSVe3HomePage.h"
#include <AmjuGL.h>

namespace Amju
{
GSVe3HomePage::GSVe3HomePage()
{
}

void GSVe3HomePage::Update()
{
  GSGui::Update();

}

void GSVe3HomePage::Draw()
{
  GSGui::Draw();

}

void GSVe3HomePage::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3HomePage::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
