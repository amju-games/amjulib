#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "SceneNode.h"

namespace Amju
{
class SceneGraph : public NonCopyable
{
public:
  SceneGraph();

  enum GraphType 
  { 
//    AMJU_CAMERA, 
    AMJU_OPAQUE,
    AMJU_SKYBOX,
    AMJU_MAX_GT
  };

  void SetRootNode(GraphType, PSceneNode root);
  PSceneNode GetRootNode(GraphType);

  void SetCamera(PSceneNode cameraNode);
  PSceneNode GetCamera();

  void Clear();  

  void Draw();
  void Update();

  void AddBlendNode(PSceneNode);

private:
  void DrawNode(SceneNode* p);
  void DrawChildren(SceneNode* p);

  void UpdateNode(SceneNode* p);
  void UpdateChildren(SceneNode* p);

private:
  // Array of nodes, for multiple passes ?
  PSceneNode m_root[AMJU_MAX_GT];

  // Pointer to the camera node
  PSceneNode m_camera;

  // Collection of alpha blended nodes
  struct BlendNode
  {
    PSceneNode m_sceneNode;
    float m_screenZ;

    bool operator<(const BlendNode& bn) const;
  };
  typedef std::vector<BlendNode> BlendNodes;
  BlendNodes m_blendNodes;
};
}

#endif
