#ifndef GS_VE3HOMEPAGE_H_INCLUDED
#define GS_VE3HOMEPAGE_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"
#include "Ve3ShowPlayer.h"

namespace Amju 
{
class GSVe3HomePage : public GSGui, public Ve3ShowPlayer
{
  GSVe3HomePage();
  friend class Singleton<GSVe3HomePage>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

protected:
};

typedef Singleton<GSVe3HomePage> TheGSVe3HomePage;
} // namespace
#endif
