#include "GSFinish.h"
#include <Game.h>

namespace Amju
{
void GSFinish::Update()
{
}

void GSFinish::DrawScene()
{
}

void GSFinish::Draw2d()
{
  m_gui->Draw();
}

void GSFinish::OnActive()
{
  m_gui = LoadGui("finish-gui.txt");
  Assert(m_gui);
}
}
