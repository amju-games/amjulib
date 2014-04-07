#pragma once

#include <Singleton.h>
#include "GSGui.h"

namespace Amju
{
class GSVe3ShowTrade : public GSGui
{
  GSVe3ShowTrade();
  friend class Singleton<GSVe3ShowTrade>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

private:

};
typedef Singleton<GSVe3ShowTrade> TheGSVe3ShowTrade;
}
