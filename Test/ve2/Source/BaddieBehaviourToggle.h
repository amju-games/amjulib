#ifndef BADDIE_BEHAVIOUR_TOGGLE_H_INCLUDED
#define BADDIE_BEHAVIOUR_TOGGLE_H_INCLUDED

#include "Baddie.h"

namespace Amju
{
class BBToggle : public BaddieBehaviour
{
public:
  BBToggle();
  virtual void Update();
  virtual bool Load(File*);

protected:
  bool m_harmful; // toggles

  // 0-1 (min, max) time in harmless state, 2-3 (min, max) time in harmful state
  float m_time[4]; 

  int m_cellRange[4]; // 0-1 harmless, 2-3 harmful cell range

  float m_timeInState;
  float m_maxTimeInState;

  std::string m_sound[2]; // harmless/harmful state change sound filename

};
}

#endif
