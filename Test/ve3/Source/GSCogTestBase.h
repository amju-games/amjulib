#ifndef GS_COG_TEST_BASE_H_INCLUDED
#define GS_COG_TEST_BASE_H_INCLUDED

#include "GSGui.h"
#include "CogTestResults.h"

namespace Amju
{
class GSCogTestBase : public GSGui
{
public:
  GSCogTestBase();

  virtual void Update();
  virtual void OnActive();

  // called when test is over - by UpdateTimer() or by test-specific code
  virtual void Finished() = 0; 

  void ShowPauseButton(bool show);
  void ShowStopPracButton(bool show);

protected:
  // Call AFTER loading m_gui
  void LoadCommonGui();

  void UpdateTimer();

  void UpdateCorrectIncorrect();

  // Call when test complete to get points added to score
  void UpdateScore();

  void PointPracArrow(const Vec2f& pos);
  void ShowPracArrow(bool show);

  // Convert time value (secs) to string "min:sec"
  static std::string TimeToString(float time);

protected:
  TestId m_testId;
  float m_timer;
  float m_maxTime;
  bool m_isFinished;
  int m_correct;
  int m_incorrect;
};
};

#endif
