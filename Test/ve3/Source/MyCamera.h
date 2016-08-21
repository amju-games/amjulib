#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <SceneNodeCamera.h>
#include <EventListener.h>
#include "Ve1Object.h"

namespace Amju
{
// For edit mode and debugging
class CameraControl : public EventListener
{
public:
  CameraControl();

  virtual bool OnKeyEvent(const KeyEvent&);
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

};

class MyCamera : public SceneNodeCamera
{
public:
  MyCamera();

  void SetTarget(Ve1Object* g) { m_target = g; }
  virtual void Update();

  void Reset();

private:
  Ve1Object* m_target;
  RCPtr<CameraControl> m_control;
};
}

#endif


