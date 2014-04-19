#ifndef GS_DEATH_H_INCLUDED
#define GS_DEATH_H_INCLUDED

#include <Singleton.h>
#include "GSMain.h"

namespace Amju
{
class GSDeath : public GSMain
{
private:
  GSDeath();
  friend class Singleton<GSDeath>;

public:
  virtual void Update();
  virtual void OnActive();

};

typedef Singleton<GSDeath> TheGSDeath;
}

#endif


