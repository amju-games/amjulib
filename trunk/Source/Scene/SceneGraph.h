#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "Singleton.h"
#include "SceneNode.h"

namespace Amju
{
class SceneGraph : public NonCopyable
{
public:
  SceneGraph();

  enum GraphType 
  { 
    AMJU_CAMERA, 
    AMJU_OPAQUE,
//    AMJU_BLENDED, 
    AMJU_SKYBOX,
    AMJU_MAX_GT
  };

  void SetRootNode(GraphType, PSceneNode root);
  PSceneNode GetRootNode(GraphType);

  void Clear();  

  void Draw();
  void Update(); // ???

  void AddBlendNode(PSceneNode);

private:
  // TODO array of nodes, for multiple passes ?
  PSceneNode m_root[AMJU_MAX_GT];

  // Pointer to the camera node
  PSceneNode m_camera;

  // Collection of alpha blended nodes
  struct BlendNode
  {
    PSceneNode m_sceneNode;
    // Matrix m_combined; // in scene node
    float m_screenZ;

    bool operator<(const BlendNode& bn) const
    {
      return m_screenZ < bn.m_screenZ;
    }
  };
  typedef std::vector<BlendNode> BlendNodes;
  BlendNodes m_blendNodes;
};

typedef Singleton<SceneGraph> TheSceneGraph;
}

#endif
