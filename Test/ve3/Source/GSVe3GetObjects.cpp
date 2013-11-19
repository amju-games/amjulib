#include "GSVe3GetObjects.h"
#include <AmjuGL.h>

namespace Amju
{
GSVe3GetObjects::GSVe3GetObjects()
{
}

void GSVe3GetObjects::Update()
{
  GSGui::Update();

}

void GSVe3GetObjects::Draw()
{
  GSGui::Draw();

}

void GSVe3GetObjects::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3GetObjects::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
