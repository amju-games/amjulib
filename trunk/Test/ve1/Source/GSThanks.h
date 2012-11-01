#ifndef GS_THANKS_H_INCLUDED
#define GS_THANKS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSThanks : public GSGui
{
  GSThanks();
  friend class Singleton<GSThanks>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSThanks> TheGSThanks;
} // namespace
#endif
