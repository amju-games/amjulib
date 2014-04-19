#ifndef GS_FINISHEDTESTS_H_INCLUDED
#define GS_FINISHEDTESTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSFinishedTests : public GSGui
{
  GSFinishedTests();
  friend class Singleton<GSFinishedTests>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSFinishedTests> TheGSFinishedTests;
} // namespace
#endif
