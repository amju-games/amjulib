#ifndef GS_BASE_H
#define GS_BASE_H

#include <string>
#include <GameState.h>
#include "Ve1OnlineReqManager.h"
#include "Ve1Req.h"

namespace Amju
{
class GSBase : public GameState
{
public:
  GSBase();
  virtual void Update();
  virtual void Draw(); // Set up proj and mview matrices
  virtual void Draw2d();
  virtual void OnActive();

  void SetPrevState(GameState*);
  GameState* GetPrevState();

  void GoBack();  // return to prev state - must have been set!

protected:

protected:
  float m_time;
  float m_maxTime;
  GameState* m_nextState;
  float m_yrot; 
  bool m_drag;
  GameState* m_prevState;
};
}

#endif
