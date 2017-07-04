#ifndef SCENE_NODE_CAMERA_H
#define SCENE_NODE_CAMERA_H

#include <Camera.h> // in AmjuGL
#include "SceneNode.h"

namespace Amju
{
class SceneNodeCamera : public SceneNode
{
public:
  static const char* NAME;

  SceneNodeCamera();
  virtual void Draw() override;
  virtual bool Load(File*) override;

  void SetEyePos(const Vec3f&);
  void SetLookAtPos(const Vec3f&);
  void SetUpVec(const Vec3f&);

  void SetFromCamera(const Camera& c);

  const Vec3f& GetEyePos() const;
  const Vec3f& GetLookAtPos() const;

  // Calc view transformation matrix from view dir and up vector
  // (like in gluLookAt)
  Matrix GetMatrix() const;

protected:
  Vec3f m_eye;
  Vec3f m_lookat;
  Vec3f m_up;
  // fovy, aspect, near, far
  float m_perspective[4];
};

typedef RCPtr<SceneNodeCamera> PSceneNodeCamera;
}

#endif
