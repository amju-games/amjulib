#include "GSVe3ViewOtherPlayers.h"
#include <AmjuGL.h>

namespace Amju
{
GSVe3ViewOtherPlayers::GSVe3ViewOtherPlayers()
{
}

void GSVe3ViewOtherPlayers::Update()
{
  GSGui::Update();

}

void GSVe3ViewOtherPlayers::Draw()
{
  GSGui::Draw();

}

void GSVe3ViewOtherPlayers::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3ViewOtherPlayers::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
