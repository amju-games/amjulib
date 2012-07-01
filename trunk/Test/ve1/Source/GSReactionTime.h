#ifndef GS_REACTIONTIME_H_INCLUDED
#define GS_REACTIONTIME_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSReactionTime : public GSGui
{
  GSReactionTime();
  friend class Singleton<GSReactionTime>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnButton();

private:
  void SetTest();

private:
  float m_reactionTime; // increase from zero 
  float m_waitTime; // elapsed time
  float m_maxWaitTime; // when m_waitTime reaches this limit we display the stimulus and start timing
  int m_testNum; // we run the test multiple times and get average 
  int m_maxTestNum; // the number of times we run the test
  enum Mode
  {
    RT_BEFORE_TEST,
    RT_WAITING,
    RT_TIMING,
    RT_AFTER_TEST,
    RT_CONTINUE,
  };
  Mode m_mode;
};
typedef Singleton<GSReactionTime> TheGSReactionTime;
} // namespace
#endif
