#include "GSCogTestBase.h"

namespace Amju
{
void GSCogTestBase::OnActive()
{
  // Skip GSGui::OnActive
  GSBase::OnActive();
  LoadCogTestBg();
}
}


