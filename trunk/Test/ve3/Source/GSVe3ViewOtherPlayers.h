#ifndef GS_VE3VIEWOTHERPLAYERS_H_INCLUDED
#define GS_VE3VIEWOTHERPLAYERS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSVe3ViewOtherPlayers : public GSGui
{
  GSVe3ViewOtherPlayers();
  friend class Singleton<GSVe3ViewOtherPlayers>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSVe3ViewOtherPlayers> TheGSVe3ViewOtherPlayers;
} // namespace
#endif
