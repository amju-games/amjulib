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

static void OnBack()
{
  TheGSVe3ViewOtherPlayers::Instance()->GoBack();
}

void GSVe3ViewOtherPlayers::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-viewotherplayers.txt");
  Assert(m_gui);

  GetElementByName(m_gui, "back-button")->SetCommand(OnBack);

}

} // namespace
