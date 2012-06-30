#ifndef GS_STROOPCOLOURWORD_H_INCLUDED
#define GS_STROOPCOLOURWORD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopColourWord : public GSGui
{
  GSStroopColourWord();
  friend class Singleton<GSStroopColourWord>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnLeftRight(bool isLeftButton);
  void Finished();
 
private:
  void SetTest();

private:
  float m_timer;
  float m_maxTime;
  bool m_leftIsCorrect;
  int m_correct;
  int m_incorrect;
  std::string m_testName;
  bool m_isFinished;
};
typedef Singleton<GSStroopColourWord> TheGSStroopColourWord;
} // namespace
#endif
