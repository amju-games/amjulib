#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <SceneNodeCamera.h>
#include <EventListener.h>
#include <GameObject.h>

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

class Camera : public SceneNodeCamera
{
public:
  Camera();

  void SetTarget(GameObject* g) { m_target = g; }
  virtual void Update();

private:
  GameObject* m_target;
  RCPtr<CameraControl> m_control;
};
}

#endif


