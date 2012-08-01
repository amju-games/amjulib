#include "GSProxy.h"
#include <AmjuGL.h>

namespace Amju
{
GSProxy::GSProxy()
{
}

void GSProxy::Update()
{
  GSGui::Update();

}

void GSProxy::Draw()
{
  GSGui::Draw();

}

void GSProxy::Draw2d()
{
  GSGui::Draw2d();
}

void GSProxy::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-proxy.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
