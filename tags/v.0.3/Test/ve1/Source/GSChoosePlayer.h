#ifndef GS_CHOOSEPLAYER_H_INCLUDED
#define GS_CHOOSEPLAYER_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSChoosePlayer : public GSGui
{
  GSChoosePlayer();
  friend class Singleton<GSChoosePlayer>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSChoosePlayer> TheGSChoosePlayer;
} // namespace
#endif
