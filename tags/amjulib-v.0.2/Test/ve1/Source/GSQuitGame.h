#ifndef GS_QUITGAME_H_INCLUDED
#define GS_QUITGAME_H_INCLUDED

#include <Singleton.h>
#include "GSGui.h"

namespace Amju 
{
class GSQuitGame : public GSGui
{
  GSQuitGame();
  friend class Singleton<GSQuitGame>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSQuitGame> TheGSQuitGame;
} // namespace
#endif
