#include "GSVe3ShowTrade.h"

namespace Amju
{
void GSVe3ShowTrade::Update()
{
  GSGui::Update();
}

void GSVe3ShowTrade::Draw()
{
  GSGui::Draw();
}

void GSVe3ShowTrade::Draw2d()
{
  GSGui::Draw2d();
}

void GSVe3ShowTrade::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-ve3-showtrade.txt");
  Assert(m_gui);

}

}
