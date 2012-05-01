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

  void SetStartLoc(int loc) { m_startLoc = loc; }
  int GetStartLoc() const { return m_startLoc; }

private:
  int m_startLoc;
};
typedef Singleton<GSStartGame> TheGSStartGame;
} // namespace
#endif
