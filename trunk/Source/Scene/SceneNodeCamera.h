#ifndef SCENE_NODE_CAMERA_H
#define SCENE_NODE_CAMERA_H

#include "SceneNode.h"

namespace Amju
{
class SceneNodeCamera : public SceneNode
{
public:
  SceneNodeCamera();
  virtual void Draw();
  virtual void Update() = 0;

protected:
  Vec3f m_eye;
  Vec3f m_lookat;
  Vec3f m_up;
};

typedef RCPtr<SceneNodeCamera> PSceneNodeCamera;
}

#endif
