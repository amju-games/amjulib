#ifndef GS_COG_TEST_BASE_H_INCLUDED
#define GS_COG_TEST_BASE_H_INCLUDED

#include "GSGui.h"

namespace Amju
{
class GSCogTestBase : public GSGui
{
public:
  virtual void OnActive();

protected:
  // Call AFTER loading m_gui
  void LoadCommonGui();
};
};

#endif
