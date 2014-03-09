#ifndef GS_REACTIONTIME_H_INCLUDED
#define GS_REACTIONTIME_H_INCLUDED

#include <Singleton.h>
#include "GSCogTestBase.h"

namespace Amju 
{
class GSReactionTime : public GSCogTestBase
{
  GSReactionTime();
  friend class Singleton<GSReactionTime>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual void Finished();

  void OnButton();
  void NextGo();
  void SetTest();
  void Quit();

private:
  float m_reactionTime; // increase from zero 
  float m_waitTime; // elapsed time
  float m_maxWaitTime; // when m_waitTime reaches this limit we display the stimulus and start timing
  int m_testNum; // we run the test multiple times and get average 
  int m_maxTestNum; // the number of times we run the test
  enum Mode
  {
    RT_NOT_STARTED_YET,
    RT_BEFORE_TEST, // display "Ready...", wait 3 secs
    RT_WAITING, // wait for random time
    RT_TIMING,  // counting elapsed time until button pressed
    RT_CONTINUE, // ready to go to next test
  };
  Mode m_mode;

  // Count "bad" clicks (clicks made before the stimulus was presented) - could be used to
  //  detect attempted cheating or misunderstanding
  int m_numBadClicks;
};
typedef Singleton<GSReactionTime> TheGSReactionTime;
} // namespace
#endif
