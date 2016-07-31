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
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void OnActive() override;

  virtual void CreateTweakMenu() override;
};
typedef Singleton<GSLighting> TheGSLighting;
} // namespace
#endif
