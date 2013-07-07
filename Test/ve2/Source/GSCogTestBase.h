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

  void UpdateCorrectIncorrect();

  // called when test is over - by UpdateTimer() or by test-specific code
  virtual void Finished() = 0; 

  // Call when test complete to get points added to score
  void UpdateScore();

protected:
  int m_testId;
  float m_timer;
  float m_maxTime;
  bool m_isFinished;
  int m_correct;
  int m_incorrect;
};
};

#endif
