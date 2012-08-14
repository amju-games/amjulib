#ifndef GS_STROOPCOLOUR_H_INCLUDED
#define GS_STROOPCOLOUR_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSStroopColour : public GSGui
{
  GSStroopColour();
  friend class Singleton<GSStroopColour>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  void OnLeftRight(bool isLeftButton);
  void Finished();
  void ResetTest(); 

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
  Colour m_goodColour;
};
typedef Singleton<GSStroopColour> TheGSStroopColour;
} // namespace
#endif
