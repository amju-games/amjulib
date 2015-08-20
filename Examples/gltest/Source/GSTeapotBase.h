#ifndef GS_TEAPOTBASE_H_INCLUDED
#define GS_TEAPOTBASE_H_INCLUDED

#include <Singleton.h>
#include "GSBase.h"

namespace Amju 
{
class GSTeapotBase : public GSBase
{
protected:
  GSTeapotBase();

public:
  virtual void Update() override;
  virtual void CreateTweakMenu() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

protected:
  void DrawTeapot();
  
protected:
  bool m_rotateX;
  bool m_rotateY;
  float m_xRot;
  float m_yRot;
  float m_xRotVel;
  float m_yRotVel;
};
} // namespace
#endif
