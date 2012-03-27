#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <SceneNodeCamera.h>
#include <EventListener.h>
#include <GameObject.h>

namespace Amju
{
class Camera : public SceneNodeCamera, public EventListener
{
public:
  Camera();

  void SetTarget(GameObject* g) { m_target = g; }
  virtual void Update();

  virtual bool OnKeyEvent(const KeyEvent&);
  virtual bool OnCursorEvent(const CursorEvent&);
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&);

private:
  GameObject* m_target;
};
}

#endif


