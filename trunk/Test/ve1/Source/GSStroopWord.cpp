#include "GSStroopWord.h"
#include <AmjuGL.h>

namespace Amju
{
GSStroopWord::GSStroopWord()
{
}

void GSStroopWord::Update()
{
  GSGui::Update();

}

void GSStroopWord::Draw()
{
//  GSGui::Draw();

}

void GSStroopWord::Draw2d()
{
  GSGui::Draw2d();
}

void GSStroopWord::OnActive()
{
//  GSGui::OnActive();

  GSBase::OnActive();
  AmjuGL::SetClearColour(Colour(1, 1, 1, 1));

  m_gui = LoadGui("gui-stroop-word.txt");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
