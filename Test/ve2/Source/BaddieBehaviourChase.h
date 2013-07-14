#ifndef BADDIE_BEHAVIOUR_CHASE_H_INCLUDED
#define BADDIE_BEHAVIOUR_CHASE_H_INCLUDED

#include "Baddie.h"

namespace Amju
{
class BBChase : public BaddieBehaviour
{
public:
  static const char* TYPENAME;

  BBChase();
  virtual void Update();
  virtual bool Load(File*);

protected:
  // Chase player if speed > 0.
  // (Flees player if speed < 0! -- is this useful?)
  float m_chaseSpeed;
};
}

#endif