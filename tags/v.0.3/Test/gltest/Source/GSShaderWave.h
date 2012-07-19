#ifndef GS_SHADERWAVE_H_INCLUDED
#define GS_SHADERWAVE_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSShaderWave : public GSBase
{
  GSShaderWave();
  friend class Singleton<GSShaderWave>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);
};
typedef Singleton<GSShaderWave> TheGSShaderWave;
} // namespace
#endif
