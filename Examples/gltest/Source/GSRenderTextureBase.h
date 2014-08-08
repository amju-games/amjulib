#pragma once

#include <Singleton.h>
#include <FullScreenPostProcess.h>
#include "GSBase.h"

namespace Amju 
{
class GSRenderTextureBase : public GSBase
{
protected:
  GSRenderTextureBase();

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

protected:
  FullScreenPostProcess m_fs;
  std::string m_shaderName;
  int m_renderFlags;
};
} // namespace

