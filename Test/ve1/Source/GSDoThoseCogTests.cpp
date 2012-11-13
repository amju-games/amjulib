#include "GSDoThoseCogTests.h"
#include <AmjuGL.h>

namespace Amju
{
GSDoThoseCogTests::GSDoThoseCogTests()
{
}

void GSDoThoseCogTests::Update()
{
  GSGui::Update();

}

void GSDoThoseCogTests::Draw()
{
  GSGui::Draw();

}

void GSDoThoseCogTests::Draw2d()
{
  GSGui::Draw2d();
}

void GSDoThoseCogTests::OnActive()
{
  GSGui::OnActive();

  m_gui = LoadGui("TODO");
  Assert(m_gui);

  // TODO Set focus element, cancel element, command handlers
}

} // namespace
