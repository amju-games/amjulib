#ifndef GS_SHADERWAVE_H_INCLUDED
#define GS_SHADERWAVE_H_INCLUDED

#include <Singleton.h>
#include <Shader.h>
#include "GSBase.h"

namespace Amju 
{
class GSShaderWave : public GSBase
{
  GSShaderWave();
  friend class Singleton<GSShaderWave>;

public:
  virtual void Update();
  virtual void DrawScene();
  virtual void Draw2d();
  virtual void OnActive();

private:
  Shader* m_shader;

};
typedef Singleton<GSShaderWave> TheGSShaderWave;
} // namespace
#endif
