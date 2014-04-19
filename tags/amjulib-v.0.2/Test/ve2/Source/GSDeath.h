#ifndef GS_DEATH_H_INCLUDED
#define GS_DEATH_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju
{
class GSDeath : public GSGui
{
private:
  GSDeath();
  friend class Singleton<GSDeath>;

public:
  virtual void Update();
  virtual void OnActive();
  virtual void OnDeactive();

};

typedef Singleton<GSDeath> TheGSDeath;
}

#endif


