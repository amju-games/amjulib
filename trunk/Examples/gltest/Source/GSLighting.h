#ifndef GS_LIGHTING_H_INCLUDED
#define GS_LIGHTING_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSLighting : public GSBase
{
  GSLighting();
  friend class Singleton<GSLighting>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void Draw2d();
  virtual void OnActive();

  virtual void SetUpTweakMenu() override;
};
typedef Singleton<GSLighting> TheGSLighting;
} // namespace
#endif
