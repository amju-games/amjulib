#ifndef GS_MAIN_H
#define GS_MAIN_H

#include <Texture.h>
#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
class GSMain : public GSBase
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
