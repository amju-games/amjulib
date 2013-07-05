#ifndef GS_COG_TEST_BASE_H_INCLUDED
#define GS_COG_TEST_BASE_H_INCLUDED

#include "GSGui.h"

namespace Amju
{
class GSCogTestBase : public GSGui
{
public:
  GSCogTestBase();

  virtual void OnActive();

protected:
  // Call AFTER loading m_gui
  void LoadCommonGui();

  void UpdateTimer();

  // called when test is over - by UpdateTimer() or by test-specific code
  virtual void Finished() = 0; 

protected:
  int m_testId;
  float m_timer;
  bool m_isFinished;
};
};

#endif
