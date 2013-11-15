#ifndef GS_PAUSE_COG_TEST_H_INCLUDED
#define GS_PAUSE_COG_TEST_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSPauseCogTest : public GSGui
{
  GSPauseCogTest();
  friend class Singleton<GSPauseCogTest>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
};
typedef Singleton<GSPauseCogTest> TheGSPauseCogTest;
} // namespace
#endif
