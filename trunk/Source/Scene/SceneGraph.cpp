#include "SceneGraph.h"
#include "SceneMesh.h"
#include <DrawAABB.h>

#define USE_VFC
//#define VFC_DEBUG
//#define VFC_DEBUG_TEXT
//#define HIERARCHY_DEBUG

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
  m_isHeirarchy = false;
}

PSceneNode SceneGraph::GetRootNode(GraphType gt)
{
  return m_root[gt];
}

void SceneGraph::SetRootNode(GraphType gt, PSceneNode root)
{
  m_root[gt] = root;
}

void SceneGraph::SetCamera(PSceneNodeCamera cameraNode)
{
  m_camera = cameraNode;
}

PSceneNodeCamera SceneGraph::GetCamera()
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
  // TODO Is this correct ?? Isn't it distance from pos to origin ?

  //const Matrix& camMat = GetCamera()->m_combined;
  //Vec3f camPos(camMat[12], camMat[13], camMat[14]);

  const Matrix& mat = p->m_combined;
  Vec3f pos(mat[12], mat[13], mat[14]);

  Vec3f camPos = GetCamera()->GetEyePos(); 

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
#ifdef HIERARCHY_DEBUG
  const AABB& aabb = *(p->GetAABB());
  Frustum::FrustumResult fr = m_frustum.Intersects(aabb);
  Assert(fr != Frustum::AMJU_OUTSIDE);
#endif // HIERARCHY_DEBUG

  m_nodesInFrustum++;

  if (p && p->IsVisible())
  {
    PushColour();
    MultColour(p->m_colour);
    p->Draw();

    if (p->ShowAABB()) 
    {
      DrawAABB(*(p->GetAABB()));
    }

    PopColour();
  }
}

void SceneGraph::DrawChildren(
  SceneNode* node, Frustum::FrustumResult fr)
{
  // If a node is not visible, its children should also not be visible. Right ?
  if (!node->IsVisible())
  {
    return;
  }

  Assert(!m_isHeirarchy || fr != Frustum::AMJU_OUTSIDE);

  // If this node is a camera, set the SceneGraph current camera to this..?
  if (node->IsCamera())
  {
    Assert(dynamic_cast<SceneNodeCamera*>(node));
    SetCamera((SceneNodeCamera*)node);
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
    if (!child->IsVisible())
    {
      continue;
    }

    // Get frustum result for child. If child nodes are always contained
    //  within their parents, we only need to recalc if the parent node
    //  is partly in and partly outside the frustum (heirarchical).
    // If not heirarchical, do the frustum test every time.
    Frustum::FrustumResult childFr = fr;
    if (!m_isHeirarchy || fr == Frustum::AMJU_PART_INSIDE)
    {
      // Need to do frustum test for child
      childFr = m_frustum.Intersects(*(child->GetAABB()));
    }

    // TODO If a node is not visible, are all children automatically
    //  invisible too ?
#ifdef USE_VFC
    if (childFr != Frustum::AMJU_OUTSIDE && child->IsVisible())
#else
    if (child->IsVisible())
#endif
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
#ifdef USE_VFC
    if (childFr != Frustum::AMJU_OUTSIDE && !child->m_children.empty())
#else
    if (!child->m_children.empty())
#endif
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

    // TODO! This is broken! Test and fix VFC!!!
    m_frustum.Create(); 
  }

  Assert(m_root[AMJU_OPAQUE]);

  // TODO We only need a stack, this is overkill I think.
  m_root[AMJU_OPAQUE]->CombineTransform();

  SceneNode* node = m_root[AMJU_OPAQUE];
  Assert(node);
  Frustum::FrustumResult fr = m_frustum.Intersects(*(node->GetAABB()));

  if (fr != Frustum::AMJU_OUTSIDE)
  {
    DrawNode(node);
    DrawChildren(node, fr);  
  }
  else if (!m_isHeirarchy)
  {
    // Children may still be in
    DrawChildren(node, fr);  
  }

  // Don't want lighting for sky, shadows or HUD, right ?
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  // Don't want this either, right ?
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

#ifdef VFC_DEBUG
  DrawAABBs(m_root[AMJU_OPAQUE]);
#endif 

  if (m_root[AMJU_SKYBOX])
  {
    // Centre on camera
    if (m_camera)
    {
      // Translate to camera eye pos 
      // (We pop matrix after drawing skybox, no additional push/pop required)
      Vec3f pos = m_camera->GetEyePos();
      AmjuGL::Translate(pos.x, pos.y, pos.z);
    }

    DrawNode(m_root[AMJU_SKYBOX]);
    DrawChildren(m_root[AMJU_SKYBOX], Frustum::AMJU_INSIDE); // TODO
  }

  AmjuGL::PopMatrix();
  AmjuGL::PushMatrix();

  // Do camera transformation again for blended nodes
  // TODO Better to remember the transformation we got the first time, in case
  //  there are multiple cameras ?
  if (m_camera)
  {
    m_camera->Draw();
  }

  // Draw alpha-blended nodes
  std::sort(m_blendNodes.begin(), m_blendNodes.end());
  
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  for (BlendNodes::iterator it = m_blendNodes.begin(); it != m_blendNodes.end(); ++it)
  {
    PSceneNode sn = it->m_sceneNode; 

    Frustum::FrustumResult fr = m_frustum.Intersects(*(sn->GetAABB()));
    if (fr != Frustum::AMJU_OUTSIDE)
    {
      AmjuGL::PushMatrix();
      // Blended nodes may need to mult by their combined transform.
      // Shadows don't need to do this because they use absolute coords.
      sn->BeforeDraw();
      DrawNode(sn);
      sn->AfterDraw();
      AmjuGL::PopMatrix();
    }
  }
  AmjuGL::PopMatrix();

#ifdef VFC_DEBUG_TEXT
  std::cout << "Nodes: " << m_nodesTotal << " in frustum: " << m_nodesInFrustum  << "\n";
#endif 

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
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
