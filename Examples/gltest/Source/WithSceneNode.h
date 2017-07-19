#pragma once

#include <SceneNode.h>

namespace Amju
{
// Mixin class for Game Objects which have a scene graph node
class WithSceneNode          
{
public:
  WithSceneNode() : m_sceneNode(nullptr) {}
  virtual ~WithSceneNode() {}

  SceneNode* GetSceneNode();
  void SetSceneNode(RCPtr<SceneNode>);
  void AddSceneNodeToGraph();
  void RemoveSceneNodeFromGraph();

protected:
  // Provide in subclass
  virtual SceneGraph* GetSceneGraph() = 0;

  // Create appropriate scene node type, load mesh and texture, etc.
  // Override for special scene nodes types like characters.
  virtual bool CreateSceneNode(const std::string& sceneFilename);

  RCPtr<SceneNode> m_sceneNode;
};
}

