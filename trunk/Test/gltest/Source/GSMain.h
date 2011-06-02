#ifndef GS_MAIN_H
#define GS_MAIN_H

#include <GameState.h>
#include <Texture.h>
#include <Singleton.h>

namespace Amju
{
class GSMain : public GameState
{
  GSMain();
  friend class Singleton<GSMain>;

public:

  // GameState overrides
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  // EventListener overrides

protected:
  Texture* m_tex;
};

typedef Singleton<GSMain> TheGSMain;
}

#endif
