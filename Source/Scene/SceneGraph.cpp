#include "SceneGraph.h"
#include "SceneMesh.h"

namespace Amju
{
bool SceneGraph::BlendNode::operator<(const BlendNode& bn) const
{
  return m_screenZ > bn.m_screenZ;
}

SceneGraph::SceneGraph()
{
}

PSceneNode SceneGraph::GetRootNode(GraphType gt)
{
  return m_root[gt];
}

void SceneGraph::SetRootNode(GraphType gt, PSceneNode root)
{
  m_root[gt] = root;
}

void SceneGraph::SetCamera(PSceneNode cameraNode)
{
  m_camera = cameraNode;
}

PSceneNode SceneGraph::GetCamera()
{
  return m_camera;
}

void SceneGraph::Clear()
{
  for (int i = 0; i < (int)AMJU_MAX_GT; i++)
  {
    m_root[i] = 0;
  }
  m_camera = 0;
}

void SceneGraph::AddBlendNode(PSceneNode p)
{
  BlendNode bn;
  bn.m_sceneNode = p;

  // Get dist to camera
  const Matrix& camMat = GetCamera()->m_combined;
  Vec3f camPos(camMat[12], camMat[13], camMat[14]);
  const Matrix& mat = p->m_combined;
  Vec3f pos(mat[12], mat[13], mat[14]);

  bn.m_screenZ = (camPos - pos).SqLen(); // ..??
  m_blendNodes.push_back(bn);
}

void SceneGraph::DrawNode(SceneNode* p)
{
  if (p && p->IsVisible())
  {
    PushColour();
    MultColour(p->m_colour);
    p->Draw();
    PopColour();
  }
}

void SceneGraph::DrawChildren(SceneNode* node)
{
  // If this node is a camera, set the SceneGraph current camera to this..?
  if (node->IsCamera())
  {
    SetCamera(node);
  }

  node->BeforeDraw(); 

  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(node->m_local);

  PushColour();
  MultColour(node->m_colour);

  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& child = node->m_children[i];

    // TODO If a node is not visible, are all children automatically
    //  invisible too ?
    if (child->IsVisible())
    {
      if (child->IsBlended())
      {
        // Add to blend list
        AddBlendNode(child);
      }
      else
      {
        DrawNode(child);
      }
    }

    // Draw children even if this node is invisible
    DrawChildren(child);
  }  

  PopColour();
  AmjuGL::PopMatrix();

  node->AfterDraw();
}

void SceneGraph::Draw()
{
  m_blendNodes.clear();

  AmjuGL::PushMatrix();

  // TODO SORT THIS OUT
  // Camera(s) are nodes in the OPAQUE tree ??
  // Multiple cameras are ok (allowing mirrors ?) 
  // The current camera is set when DrawChildren() discovers that the
  // current node is a camera.
  // Not: m_camera->Draw();

  if (m_camera)
  {
    m_camera->Draw();
  }

  Assert(m_root[AMJU_OPAQUE]);

  m_root[AMJU_OPAQUE]->CombineTransform();

  DrawNode(m_root[AMJU_OPAQUE]);
  DrawChildren(m_root[AMJU_OPAQUE]);

  // Don't want lighting for sky, shadows or HUD, right ?
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  if (m_root[AMJU_SKYBOX])
  {
    // Centre on camera
    if (m_camera)
    {
      // Translate to camera eye pos ?     
      //m_camera->Draw(); // ??????
    }

    DrawNode(m_root[AMJU_SKYBOX]);
    DrawChildren(m_root[AMJU_SKYBOX]);
  }

  AmjuGL::PopMatrix();
  AmjuGL::PushMatrix();

  // Do camera transformation again for blended nodes
  if (m_camera)
  {
    m_camera->Draw();
  }

  // Draw alpha-blended nodes
  std::sort(m_blendNodes.begin(), m_blendNodes.end());

  for (BlendNodes::iterator it = m_blendNodes.begin(); it != m_blendNodes.end(); ++it)
  {
    PSceneNode sn = it->m_sceneNode; 

    AmjuGL::PushMatrix();
    // Blended nodes may need to mult by their combined transform.
    // Shadows don't need to do this because they use absolute coords.
    DrawNode(sn);
    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();
}

void SceneGraph::UpdateNode(SceneNode* node)
{
  if (node)
  {
    node->Update();
  }
}

void SceneGraph::UpdateChildren(SceneNode* node)
{
  if (!node)
  {
    return;
  }

  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& child = node->m_children[i];
    UpdateNode(child);
    UpdateChildren(child);
  }  
}

void SceneGraph::Update()
{
  Assert(m_root[AMJU_OPAQUE]);
  UpdateNode(m_root[AMJU_OPAQUE]);
  UpdateChildren(m_root[AMJU_OPAQUE]);

  if (m_root[AMJU_SKYBOX])
  {
    UpdateNode(m_root[AMJU_SKYBOX]);
    UpdateChildren(m_root[AMJU_SKYBOX]);
  }
}
}
