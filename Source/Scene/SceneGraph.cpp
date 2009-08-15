#include "SceneGraph.h"
#include "SceneComposite.h" // Bah, dependency bottleneck
#include "SceneMesh.h"

namespace Amju
{
SceneNode* CreateRoot() { return new SceneNode; }
SceneNode* CreateSceneMesh() { return new SceneMesh; }

SceneGraph::SceneGraph()
{
  TheSceneNodeFactory::Instance()->Add(SceneNode::NAME, &CreateRoot);
  TheSceneNodeFactory::Instance()->Add(SceneMesh::NAME, &CreateSceneMesh);

}

void SceneGraph::SetRootNode(GraphType gt, PSceneNode root)
{
  m_root[gt] = root;
}

void SceneGraph::Clear()
{
  for (int i = 0; i < (int)AMJU_MAX_GT; i++)
  {
    m_root[i] = 0;
  }
}

PSceneNode SceneGraph::GetRootNode(GraphType gt)
{
  return m_root[gt];
}

void SceneGraph::AddBlendNode(PSceneNode p)
{
  BlendNode bn;
  bn.m_sceneNode = p;
  bn.m_screenZ = 0; //GetCurrentCamera()->GetPos() - p->GetPos() // ..??
  m_blendNodes.push_back(bn);
}

void SceneGraph::Draw()
{
  m_blendNodes.clear();

  AmjuGL::PushMatrix();

  // Camera(s) are nodes in the OPAQUE tree.
  // Multiple cameras are ok (allowing mirrors ?) 
  // The current camera is set when DrawChildren() discovers that the
  // current node is a camera.
  // Not: m_camera->Draw();

  Assert(m_root[AMJU_OPAQUE]);
  m_root[AMJU_OPAQUE]->Draw();
  m_root[AMJU_OPAQUE]->DrawChildren();

  // Don't want lighting for sky, shadows or HUD, right ?
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  if (m_root[AMJU_SKYBOX])
  {
    m_root[AMJU_SKYBOX]->Draw();
    m_root[AMJU_SKYBOX]->DrawChildren();
  }

  AmjuGL::PopMatrix();

  // Draw alpha-blended nodes
  std::sort(m_blendNodes.begin(), m_blendNodes.end());

  for (BlendNodes::iterator it = m_blendNodes.begin(); it != m_blendNodes.end(); ++it)
  {
    AmjuGL::PushMatrix();
    PSceneNode sn = it->m_sceneNode; 

    if (sn->GetParent())
    {
      AmjuGL::MultMatrix(sn->GetParent()->GetCombinedTransform());
    }
    sn->Draw(); // concats local transform
    AmjuGL::PopMatrix();
  }
}

void SceneGraph::Update()
{
  Assert(m_root[AMJU_OPAQUE]);
  m_root[AMJU_OPAQUE]->Update();
  m_root[AMJU_OPAQUE]->UpdateChildren();

  if (m_root[AMJU_SKYBOX])
  {
    m_root[AMJU_SKYBOX]->Update();
    m_root[AMJU_SKYBOX]->UpdateChildren();
  }
}
}
