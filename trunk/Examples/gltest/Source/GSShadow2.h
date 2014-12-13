#pragma once

#include <Singleton.h>
#include <Shader.h>
#include "GSRenderTextureBase.h"

namespace Amju 
{
class GSShadow2 : public GSRenderTextureBase
{
private:
  GSShadow2();
  friend class Singleton<GSShadow2>;

public:
  void Draw() override;
  void OnActive() override;

private:
  RCPtr<Shader> m_secondPassShader;
};
typedef Singleton<GSShadow2> TheGSShadow2;
} // namespace

