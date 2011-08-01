#include "GSEnterName.h"

namespace Amju
{
void GSEnterName::Update()
{
  m_gui->Update();
}

void GSEnterName::Draw()
{
}

void GSEnterName::Draw2d()
{
  m_gui->Draw();
}

void GSEnterName::OnActive()
{
  m_gui = LoadGui("enter-name-gui.txt");
  Assert(m_gui);
}

void GSEnterName::OnDeactive()
{
  m_gui = 0;
}
} 
