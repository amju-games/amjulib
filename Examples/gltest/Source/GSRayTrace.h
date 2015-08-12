#ifndef GS_RAYTRACE_H_INCLUDED
#define GS_RAYTRACE_H_INCLUDED

#include <Singleton.h>
#include <Texture.h>
#include "GSBase.h"

namespace Amju 
{
class GSRayTrace : public GSBase
{
  GSRayTrace();
  friend class Singleton<GSRayTrace>;

public:
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void CreateTweakMenu() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

private:
  FullScreenPostProcess m_fspp;
  Texture m_tex;
};
typedef Singleton<GSRayTrace> TheGSRayTrace;
} // namespace
#endif
