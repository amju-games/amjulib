#include "SceneNode.h"
#include "File.h"
#include "StringUtils.h"
#include "LoadMatrix.h"
#include "AmjuGL.h"
#include "SceneGraph.h"

namespace Amju
{
const char* SceneNode::NAME = "comp";

PSceneNode LoadScene(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return 0;
  }
  std::string s;
  if (!f.GetDataLine(&s))
  {
    f.ReportError("Expected scene node type");
    return 0;
  }
  PSceneNode node = TheSceneNodeFactory::Instance()->Create(s);
  if (!node)
  {
    f.ReportError("Failed to create node; type: " + s);
    return 0;
  }
  if (!node->Load(&f))
  {
    return 0;
  }
  return node;
}

SceneNode::SceneNode()
{
  m_parent = 0;

  // Set flags to most common values
  m_flags = 0;
  SetBlended(false);
  SetVisible(true);
  SetIsCamera(false);
  SetCollidable(true);
}

SceneNode* SceneNode::GetParent()
{
  return m_parent;
}

void SceneNode::SetParent(SceneNode* p)
{
  m_parent = p;
}

void SceneNode::UpdateChildren()
{
  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& p = m_children[i];
    p->Update();
    p->UpdateChildren();
  }  
}

void SceneNode::DrawChildren()
{
  /*
  // If this node is a camera, set the SceneGraph current camera to this..?
  if (IsCamera())
  {
    TheSceneGraph::Instance()->SetCurrentCamera(this);
  }
  */

  BeforeDraw(); // ??

  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);

  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& p = m_children[i];
    if (p->IsBlended())
    {
      // Add to blend list
      TheSceneGraph::Instance()->AddBlendNode(p);
    }
    //else if (p->IsCamera())
    //{
    //  // Do nothing, we draw it first, right ?
    //  // TODO What about multiple cameras ?
    //}
    else
    {
      p->Draw();
    }
    
    p->DrawChildren();
  }  

  AmjuGL::PopMatrix();

  AfterDraw();
}

void SceneNode::UpdateBoundingVol()
{
  // TODO Calc updated bounding vol for this node
  // Our bounding box must contain the bounding box of all children.

  if (m_parent)
  {
    m_parent->UpdateBoundingVol();
  }
}

void SceneNode::AddChild(PSceneNode node)
{
  node->SetParent(this);
  m_children.push_back(node);
}

bool SceneNode::LoadMatrix(File* f)
{
  // Load local transform matrix
  if (!Amju::LoadMatrix(f, &m_local))
  {
    return false;
  }
  return true;
}

bool SceneNode::LoadChildren(File* f)
{
  int numChildren = 0;
  f->GetInteger(&numChildren);
  for (int i = 0; i < numChildren;  i++)
  {
    std::string s;
    f->GetDataLine(&s); // TODO Change this fn name ?
    PSceneNode p = TheSceneNodeFactory::Instance()->Create(s);
    Assert(p);
    p->Load(f);
    m_children.push_back(p);
  }
  return true;
}

bool SceneNode::Load(File* f)
{
  if (!LoadMatrix(f))
  {
    return false;
  }

  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
}

void SceneNode::MultLocalTransform(const Matrix& mat)
{
  m_local *= mat;
}

void SceneNode::SetLocalTransform(const Matrix& mat)
{
  m_local = mat;
}

const Matrix& SceneNode::GetLocalTransform() const
{
  return m_local;
}

const Matrix& SceneNode::GetCombinedTransform() const
{
  return m_combined;
}

void SceneNode::CombineTransform()
{
  if (m_parent)
  {
    m_combined = m_parent->m_combined * m_local;
  }
  else
  {
    m_combined = m_local;
  }

  unsigned int s = m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    m_children[i]->CombineTransform();
  }
}

static const uint32 VISIBLE    = 0x01;
static const uint32 COLLIDABLE = 0x02;
static const uint32 BLENDED    = 0x04;
static const uint32 CAMERA     = 0x08;

bool SceneNode::IsVisible() const
{
  return m_flags & VISIBLE;
}

bool SceneNode::IsCollidable() const
{
  return (m_flags & COLLIDABLE) != 0;
}

bool SceneNode::IsBlended() const
{
  return (m_flags & BLENDED) != 0;
}

bool SceneNode::IsCamera() const
{
  return (m_flags & CAMERA) != 0;
}

void SceneNode::SetVisible(bool b)
{
  if (b)
  {
    m_flags |= VISIBLE;
  }
  else
  {
    m_flags &= ~VISIBLE;
  }
}

void SceneNode::SetCollidable(bool b)
{
  if (b)
  {
    m_flags |= COLLIDABLE;
  }
  else
  {
    m_flags &= ~COLLIDABLE;
  }
}

void SceneNode::SetBlended(bool b)
{
  if (b)
  {
    m_flags |= BLENDED;
  }
  else
  {
    m_flags &= ~BLENDED;
  }
}

void SceneNode::SetIsCamera(bool b)
{
  if (b)
  {
    m_flags |= CAMERA;
  }
  else
  {
    m_flags &= ~CAMERA;
  }
}
}
