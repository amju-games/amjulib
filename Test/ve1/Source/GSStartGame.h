#ifndef GS_STARTGAME_H_INCLUDED
#define GS_STARTGAME_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSStartGame : public GSBase
{
  GSStartGame();
  friend class Singleton<GSStartGame>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSStartGame> TheGSStartGame;
} // namespace
#endif
