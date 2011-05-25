#include "GSFinish.h"
#include <Game.h>

namespace Amju
{
const char* GSFinish::NAME = "finish";

static bool b = TheGame::Instance()->AddState(GSFinish::NAME, new GSFinish);

void GSFinish::Update()
{
}

void GSFinish::Draw()
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
