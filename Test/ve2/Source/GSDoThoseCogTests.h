#ifndef GS_DOTHOSECOGTESTS_H_INCLUDED
#define GS_DOTHOSECOGTESTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSDoThoseCogTests : public GSGui
{
  GSDoThoseCogTests();
  friend class Singleton<GSDoThoseCogTests>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSDoThoseCogTests> TheGSDoThoseCogTests;
} // namespace
#endif
