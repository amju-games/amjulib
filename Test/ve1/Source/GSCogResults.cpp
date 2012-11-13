#include "GSCogResults.h"
#include <AmjuGL.h>

namespace Amju
{
GSCogResults::GSCogResults()
{
}

void GSCogResults::Update()
{
  GSGui::Update();

}

void GSCogResults::Draw()
{
  GSGui::Draw();

}

void GSCogResults::Draw2d()
{
  GSGui::Draw2d();
}

void GSCogResults::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("gui-cogresults.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace