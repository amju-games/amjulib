#ifndef GS_SCRIBBLE_H_INCLUDED
#define GS_SCRIBBLE_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSScribble : public GSGui
{
  GSScribble();
  friend class Singleton<GSScribble>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSScribble> TheGSScribble;
} // namespace
#endif
