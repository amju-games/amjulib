#pragma once

#include <GameState.h>
#include <Singleton.h>

namespace Amju
{
class GSSoftBody : public GameState
{
public:
  GSSoftBody();
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // TODO
//  virtual bool OnKeyEvent(const KeyEvent&);

private:
};

typedef Singleton<GSSoftBody> TheGSSoftBody;

}
