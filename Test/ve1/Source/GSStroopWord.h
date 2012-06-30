#ifndef GS_STROOPWORD_H_INCLUDED
#define GS_STROOPWORD_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopWord : public GSGui
{
  GSStroopWord();
  friend class Singleton<GSStroopWord>;

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
typedef Singleton<GSStroopWord> TheGSStroopWord;
} // namespace
#endif
