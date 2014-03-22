#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "SceneNode.h"
#include "SceneNodeCamera.h"
#include "Frustum.h"

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

  void SetCamera(PSceneNodeCamera cameraNode);
  PSceneNodeCamera GetCamera();

  void Clear();  

  void Draw();
  void Update();

  void AddBlendNode(PSceneNode);

  Frustum* GetFrustum();

private:
  void DrawNode(SceneNode* p);
  // Draw children of this node - which is either in or half-in the
  //  frustum. If fully in, no need to test children, so long as the
  //  bounding volumes are fully heirarchical. If not, all children are
  //  tested against the frustum.
  void DrawChildren(SceneNode* p, Frustum::FrustumResult fr);

  // Recursively draw AABBs - for debugging
  void DrawAABBs(SceneNode* node);

  void UpdateNode(SceneNode* p);
  void UpdateChildren(SceneNode* p);

public:
  // Collection of alpha blended nodes
  struct BlendNode
  {
    PSceneNode m_sceneNode;
    float m_screenZ; // z distance from camera
    Vec3f m_pos; // world coords

    bool operator<(const BlendNode& bn) const;
  };

  typedef bool (*BlendNodeComp)(const BlendNode&, const BlendNode&);

  void SetBlendNodeComp(BlendNodeComp bnc);

private:
  // Array of nodes, for multiple passes ?
  PSceneNode m_root[AMJU_MAX_GT];

  // Pointer to the camera node
  PSceneNodeCamera m_camera;

  typedef std::vector<BlendNode> BlendNodes;
  BlendNodes m_blendNodes;

  BlendNodeComp m_blendNodeComp;

  Frustum m_frustum;
  int m_nodesInFrustum;
  int m_nodesTotal; // total number of nodes 

  // True if all nodes' bounding vols enclose all children
  bool m_isHeirarchy;
};
}

#endif
