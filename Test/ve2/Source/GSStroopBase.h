#ifndef GS_STROOP_BASE_H_INCLUDED
#define GS_STROOP_BASE_H_INCLUDED

#include "GSCogTestBase.h"

namespace Amju
{
class GSStroopBase : public GSCogTestBase
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
  virtual void Finished();

protected:
  int m_correctChoice;
  std::string m_testName;

  static const int NUM_WORDS = 5;
  static const char* WORDS[NUM_WORDS];
  static const Colour COLOURS[NUM_WORDS];

  int m_indices[NUM_WORDS];
};
}

#endif
