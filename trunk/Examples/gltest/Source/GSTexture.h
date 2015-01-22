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
  virtual void DrawScene();
  virtual void OnActive();

};
typedef Singleton<GSTexture> TheGSTexture;
} // namespace
#endif
