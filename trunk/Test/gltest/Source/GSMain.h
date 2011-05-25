#ifndef GS_MAIN_H
#define GS_MAIN_H

#include <GameState.h>
#include <Texture.h>

namespace Amju
{
class GSMain : public GameState
{
public:
  static const char* NAME;

  GSMain();

  // GameState overrides
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // EventListener overrides

protected:
  Texture* m_tex;
};
}

#endif
