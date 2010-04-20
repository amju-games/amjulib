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

  void SetEyePos(const Vec3f&);
  void SetLookAtPos(const Vec3f&);
  void SetUpVec(const Vec3f&);

  const Vec3f& GetEyePos() const;

protected:
  Vec3f m_eye;
  Vec3f m_lookat;
  Vec3f m_up;
};

typedef RCPtr<SceneNodeCamera> PSceneNodeCamera;
}

#endif
