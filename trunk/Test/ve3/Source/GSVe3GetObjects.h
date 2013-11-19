#ifndef GS_VE3GETOBJECTS_H_INCLUDED
#define GS_VE3GETOBJECTS_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSVe3GetObjects : public GSGui
{
  GSVe3GetObjects();
  friend class Singleton<GSVe3GetObjects>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSVe3GetObjects> TheGSVe3GetObjects;
} // namespace
#endif
