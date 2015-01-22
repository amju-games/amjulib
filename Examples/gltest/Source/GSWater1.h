#pragma once

#include <Singleton.h>
#include <Shader.h>
#include "GSBase.h"

namespace Amju 
{
class GSWater1 : public GSBase
{
  GSWater1();
  friend class Singleton<GSWater1>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void OnActive();
  virtual void SetUpTweakMenu() override;

private:
  Shader* m_shader;

};
typedef Singleton<GSWater1> TheGSWater1;
} // namespace

