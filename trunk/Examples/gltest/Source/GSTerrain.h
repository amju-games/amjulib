#pragma once

#include <Singleton.h>
#include <Shader.h>
#include <Texture.h>
#include "GSBase.h"

namespace Amju 
{
class GSTerrain : public GSBase
{
  GSTerrain();
  friend class Singleton<GSTerrain>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void Draw2d();
  virtual void OnActive();

private:
  Shader* m_shader;
  Texture* m_heightmap;
  Texture* m_diffuse;
  Texture* m_detail;
};
typedef Singleton<GSTerrain> TheGSTerrain;
} // namespace

