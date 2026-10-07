// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <ShadowMap.h>
#include <Shader.h>
#include <RenderToTexture.h>
#include <Camera.h>

namespace Amju
{
class ShadowMapOpenGL : public ShadowMap
{
public:
  virtual bool Init();
  virtual void Draw();

private:
};

}

