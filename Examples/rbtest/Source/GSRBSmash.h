#pragma once

#include <Vec2.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSRBSmash : public GSBase
{
public:
  GSRBSmash();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnKeyEvent(const KeyEvent&);

private:
};

typedef Singleton<GSRBSmash> TheGSRBSmash;
}


