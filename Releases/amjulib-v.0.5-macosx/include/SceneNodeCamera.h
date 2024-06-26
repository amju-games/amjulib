#ifndef SCENE_NODE_CAMERA_H
#define SCENE_NODE_CAMERA_H

#include <array>
#include <Camera.h> // in AmjuGL
#include "SceneNode.h"

namespace Amju
{
// * SceneNodeCamera *
// Look-at camera, i.e. sets modelview matrix, but not projection matrix.
// Base class for perspective and ortho cameras
class SceneNodeCamera : public SceneNode
{
public:
  SceneNodeCamera();

  void SetEyePos(const Vec3f&);
  void SetLookAtPos(const Vec3f&);
  void SetUpVec(const Vec3f&);

  void SetFromCamera(const Camera& c);

  const Vec3f& GetEyePos() const;
  const Vec3f& GetLookAtPos() const;

  // Calc view transformation matrix from view dir and up vector
  // (like in gluLookAt)
  Matrix GetMatrix() const;

  virtual bool Load(File*) override;
  virtual void Draw() override;

protected:
  Vec3f m_eye;
  Vec3f m_lookat;
  Vec3f m_up;
};

class SceneNodeCameraPersp : public SceneNodeCamera
{
public:
  static const char* NAME;

  SceneNodeCameraPersp();
  virtual void Draw() override;
  virtual bool Load(File*) override;

  void SetPerspParams(float fovy, float aspeect, float nearDist, float farDist);

private:
  // fovy, aspect, near, far
  //float m_perspective[4];
  std::array<float, 4> m_perspective;
};

class SceneNodeCameraOrtho : public SceneNodeCamera
{
public:
  static const char* NAME;

  virtual void Draw() override;
  virtual bool Load(File*) override;

private:
  //float m_ortho[6];
  std::array<float, 6> m_ortho;
};

typedef RCPtr<SceneNodeCamera> PSceneNodeCamera;
}

#endif
