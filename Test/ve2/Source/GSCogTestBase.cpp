#include <GuiComposite.h>
#include "GSCogTestBase.h"

namespace Amju
{
void GSCogTestBase::OnActive()
{
  // Skip GSGui::OnActive
  GSBase::OnActive();
  LoadCogTestBg();
}

void GSCogTestBase::LoadCommonGui()
{
  // Load common gui file, then create composite of this and the 
  //  state-specific gui.
  PGuiElement common = LoadGui("gui-cog-test-common.txt");
  Assert(common);
  
  GuiComposite* comp = new GuiComposite;
  comp->AddChild(m_gui);
  comp->AddChild(common);
  m_gui = comp;
}
}


