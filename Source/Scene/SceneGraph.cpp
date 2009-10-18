#include "SceneGraph.h"
#include "SceneMesh.h"
#include <DrawAABB.h>

//#define VFC_DEBUG

namespace Amju
{
bool SceneGraph::BlendNode::operator<(const BlendNode& bn) const
{
  return m_screenZ > bn.m_screenZ;
}

SceneGraph::SceneGraph()
{
  m_nodesInFrustum = 0;
  m_nodesTotal = 0;
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
  m_nodesInFrustum = 0;
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

Frustum* SceneGraph::GetFrustum()
{
  return &m_frustum;
}

void SceneGraph::DrawAABBs(SceneNode* node)
{
  if (!node)
  {
    return;
  }

  // Good place to work out total number of nodes because we recurse
  //  over the entire tree - no culling here.
  m_nodesTotal++;

  DrawAABB(*(node->GetAABB()));
  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    SceneNode* child = node->m_children[i];
    DrawAABBs(child);
  }
}

void SceneGraph::DrawNode(SceneNode* p)
{
#ifdef _DEBUG
  const AABB& aabb = *(p->GetAABB());
  Frustum::FrustumResult fr = m_frustum.Intersects(aabb);
  Assert(fr != Frustum::AMJU_OUTSIDE);
#endif // _DEBUG

  m_nodesInFrustum++;

  if (p && p->IsVisible())
  {
    PushColour();
    MultColour(p->m_colour);
    p->Draw();
    PopColour();
  }
}

void SceneGraph::DrawChildren(SceneNode* node, Frustum::FrustumResult fr)
{
  Assert(fr != Frustum::AMJU_OUTSIDE);

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

    // Get frustum result for child. If child nodes are always contained
    //  within their parents, we only need to recalc if the parent node
    //  is partly in and partly outside the frustum. This breaks if the
    //  nodes are not hierarchically contained.
    Frustum::FrustumResult childFr = fr;
    if (fr == Frustum::AMJU_PART_INSIDE)
    {
      // Get result for child
      childFr = m_frustum.Intersects(*(child->GetAABB()));
    }

    // TODO If a node is not visible, are all children automatically
    //  invisible too ?
    if (childFr != Frustum::AMJU_OUTSIDE && child->IsVisible())
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

    // Draw children of the child node (even if child is invisible)
    // Only do this if the child node has children of course!
    if (childFr != Frustum::AMJU_OUTSIDE &&
        !child->m_children.empty())
    {
      DrawChildren(child, childFr);
    }
  }  

  PopColour();
  AmjuGL::PopMatrix();

  node->AfterDraw();
}

void SceneGraph::Draw()
{
  m_nodesInFrustum = 0;
  m_nodesTotal = 0;
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
    // TODO This should go in CameraNode::Draw
    //m_frustum.Create(); 
  }

  Assert(m_root[AMJU_OPAQUE]);

  // TODO We only need a stack, this is overkill I think.
  m_root[AMJU_OPAQUE]->CombineTransform();

  SceneNode* node = m_root[AMJU_OPAQUE];
  Frustum::FrustumResult fr = m_frustum.Intersects(*(node->GetAABB()));
  if (fr != Frustum::AMJU_OUTSIDE)
  {
    DrawNode(node);
    DrawChildren(node, fr);  
  }

  // Don't want lighting for sky, shadows or HUD, right ?
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

#ifdef VFC_DEBUG
  DrawAABBs(m_root[AMJU_OPAQUE]);
#endif 

  if (m_root[AMJU_SKYBOX])
  {
    // Centre on camera
    if (m_camera)
    {
      // Translate to camera eye pos ?     
      //m_camera->Draw(); // ??????
    }

    DrawNode(m_root[AMJU_SKYBOX]);
    DrawChildren(m_root[AMJU_SKYBOX], Frustum::AMJU_INSIDE); // TODO
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

    Frustum::FrustumResult fr = m_frustum.Intersects(*(sn->GetAABB()));
    if (fr != Frustum::AMJU_OUTSIDE)
    {
      AmjuGL::PushMatrix();
      // Blended nodes may need to mult by their combined transform.
      // Shadows don't need to do this because they use absolute coords.
      DrawNode(sn);
      AmjuGL::PopMatrix();
    }
  }
  AmjuGL::PopMatrix();

#ifdef VFC_DEBUG
  std::cout << "Nodes: " << m_nodesTotal << " in frustum: " << m_nodesInFrustum  << "\n";
#endif 
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
  UpdateNode(m_camera);
  UpdateChildren(m_camera);

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
