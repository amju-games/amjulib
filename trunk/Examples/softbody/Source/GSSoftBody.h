#pragma once

#include <GameState.h>
#include <Singleton.h>

namespace Amju
{
class GSSoftBody : public GameState
{
public:
  GSSoftBody();
  virtual void Update() override;
  virtual void Draw() override;
  virtual void Draw2d() override;
  virtual void OnActive() override;

  bool OnKeyEvent(const KeyEvent&) override;
  bool OnCursorEvent(const CursorEvent& ce) override;
  bool OnMouseButtonEvent(const MouseButtonEvent& mbe) override;

private:
};

typedef Singleton<GSSoftBody> TheGSSoftBody;

}
