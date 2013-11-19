#ifndef GS_VE3GETOBJECTSTATES_H_INCLUDED
#define GS_VE3GETOBJECTSTATES_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSVe3GetObjectStates : public GSGui
{
  GSVe3GetObjectStates();
  friend class Singleton<GSVe3GetObjectStates>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSVe3GetObjectStates> TheGSVe3GetObjectStates;
} // namespace
#endif
