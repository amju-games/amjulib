#include "SceneNode.h"
#include "File.h"
#include "StringUtils.h"
#include "LoadMatrix.h"
#include "AmjuGL.h"
#include "SceneGraph.h"

namespace Amju
{
const char* SceneNode::NAME = "comp";

SceneNode* SceneNode::Create() { return new SceneNode; }

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
  SetIsLit(false);
}

void SceneNode::SetColour(const Colour& colour)
{
  m_colour = colour;
}

AABB* SceneNode::GetAABB()
{
  return &m_aabb;
}

SceneNode* SceneNode::GetParent()
{
  return m_parent;
}

void SceneNode::SetParent(SceneNode* p)
{
  m_parent = p;
}
  
void SceneNode::RecursivelyTransformAABB(const Matrix& m)
{
#ifdef AABB_TRANSFORM_DEBUG
std::cout << "Scene node transform AABB... ";
#endif

  GetAABB()->Transform(m);
  unsigned int s = m_children.size();

#ifdef AABB_TRANSFORM_DEBUG
std::cout << s << " children... ";
#endif

  for (unsigned int i = 0; i < s; i++)
  {
#ifdef AABB_TRANSFORM_DEBUG
std::cout << " child " << i << "... ";
#endif
    
    m_children[i]->RecursivelyTransformAABB(m);
  } 
}

void SceneNode::UpdateBoundingVol()
{
  // This should be called at a leaf node. We recurse UP towards the root,
  //  adjusting the size of the parent's boxes.

  // Calc updated bounding vol for this node
  // Our bounding box must contain the bounding box of all children.
  unsigned int s = m_children.size();
  if (s > 0)
  {
    // Start off with b.box the same size as child zero
    m_aabb = *(m_children[0]->GetAABB());
    for (unsigned int i = 1; i < s; i++)
    {
      // Don't do this, we are going UP the tree.
      //m_children[i]->UpdateBoundingVol();

      // Increase our box to fit the other children
      m_aabb.Union(*(m_children[i]->GetAABB()));
    }
  }

  if (m_parent)
  {
    m_parent->UpdateBoundingVol();
  }
}

void SceneNode::DelChild(PSceneNode node)
{
  Assert(node);
  node->SetParent(0); // probably not necessary
  Nodes::iterator it = std::find(m_children.begin(), m_children.end(), node);
  if (it == m_children.end())
  {
#ifdef _DEBUG
    std::cout << "Unexpected, node not in scene graph.\n";
#endif
  }
  else
  {
    m_children.erase(it);
  }
}

void SceneNode::AddChild(PSceneNode node)
{
  Assert(node);
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

static bool s_showAABB = false;
void SceneNode::SetGlobalShowAABB(bool b)
{
  s_showAABB = b;
}

bool SceneNode::ShowAABB() const
{
  // TODO
  return s_showAABB;
}

// TODO   void SetShowAABB(bool);

static const uint32 VISIBLE    = 0x01;
static const uint32 COLLIDABLE = 0x02;
static const uint32 BLENDED    = 0x04;
static const uint32 CAMERA     = 0x08;
static const uint32 LIGHTING   = 0x10;

bool SceneNode::IsLit() const
{
  return m_flags & LIGHTING;
}

void SceneNode::SetIsLit(bool b)
{
  if (b)
  {
    m_flags |= LIGHTING;
  }
  else
  {
    m_flags &= ~LIGHTING;
  }
}

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
