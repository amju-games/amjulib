#ifndef GS_STROOP_BASE_H_INCLUDED
#define GS_STROOP_BASE_H_INCLUDED

#include "GSGui.h"

namespace Amju
{
class GSStroopBase : public GSGui
{
public:
  GSStroopBase();

  virtual void OnActive();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();

  void OnChoiceButton(int choice);

  virtual void ResetTest();
protected:
  virtual void SetTest();

  void Finished();

protected:
  float m_timer;
  float m_maxTime;
  int m_correctChoice;
  int m_correct;
  int m_incorrect;
  std::string m_testName;
  bool m_isFinished;

  int m_testId;

  static const int NUM_WORDS = 5;
  static const char* WORDS[NUM_WORDS];
  static const Colour COLOURS[NUM_WORDS];

  int m_indices[NUM_WORDS];
};
}

#endif
