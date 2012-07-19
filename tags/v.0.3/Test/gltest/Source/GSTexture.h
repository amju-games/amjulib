#ifndef GS_TEXTURE_H_INCLUDED
#define GS_TEXTURE_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSTexture : public GSBase
{
  GSTexture();
  friend class Singleton<GSTexture>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSTexture> TheGSTexture;
} // namespace
#endif
