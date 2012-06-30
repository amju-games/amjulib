#include "GSStroopColourWord.h"
#include <AmjuGL.h>

namespace Amju
{
GSStroopColourWord::GSStroopColourWord()
{
}

void GSStroopColourWord::Update()
{
  GSGui::Update();

}

void GSStroopColourWord::Draw()
{
  GSGui::Draw();

}

void GSStroopColourWord::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopColourWord::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
