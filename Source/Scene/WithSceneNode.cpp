#include "LoadScene.h"
#include "SceneGraph.h"
#include "WithSceneNode.h"

namespace Amju
{
SceneNode* WithSceneNode::GetSceneNode()
{
  return m_sceneNode;
}

void WithSceneNode::SetSceneNode(RCPtr<SceneNode> sn)
{
  m_sceneNode = sn;
}

void WithSceneNode::AddSceneNodeToGraph()
{
  Assert(m_sceneNode);

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);

  root->AddChild(m_sceneNode);
}

void WithSceneNode::RemoveSceneNodeFromGraph()
{
  if (m_sceneNode)
  {
    SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
    Assert(root);
    root->DelChild(m_sceneNode);
  }
}

bool WithSceneNode::CreateSceneNode(const std::string& sceneFilename)
{
  PSceneNode sn = LoadScene(sceneFilename);
  SetSceneNode(sn);
  return sn != nullptr;
}
}


