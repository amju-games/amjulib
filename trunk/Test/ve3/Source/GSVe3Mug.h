#pragma once

#include "GSGui.h"

namespace Amju
{
class GSVe3Mug;
typedef Singleton<GSVe3Mug> TheGSVe3Mug;

class GSVe3Mug : public GSGui
{
public:
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();
};
}


