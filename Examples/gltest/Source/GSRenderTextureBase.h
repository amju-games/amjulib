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
  virtual void Update() override;
  virtual void DrawScene() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

protected:
  FullScreenPostProcess m_fs;
  std::string m_shaderName;
  int m_renderFlags;
};
} // namespace

