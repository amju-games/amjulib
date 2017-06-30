#include <AmjuFirst.h>
#include "SceneNode.h"
#include "File.h"
#include "StringUtils.h"
#include "LoadMatrix.h"
#include "AmjuGL.h"
#include "SceneGraph.h"
#include "SceneNodeFactory.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* SceneNode::NAME = "comp";

SceneNode* SceneNode::Create() { return new SceneNode; }

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

const std::string& SceneNode::GetName() const
{
  return m_name;
}

void SceneNode::SetColour(const Colour& colour)
{
  m_colour = colour;
}

void SceneNode::SetMaterial(PSceneNodeMaterial material)
{
  m_material = material;

  // Set a uniform for each texture/colour/value in the shader
  // TODO
}

void SceneNode::DrawMaterial()
{
  if (m_material)
  {
    m_material->Draw();
  }
}

const AABB* SceneNode::GetAABB() const
{
  return &m_aabb;
}

void SceneNode::SetAABB(const AABB& aabb)
{
  m_aabb = aabb;
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

  m_aabb.Transform(m);
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
  unsigned int s = m_children.size();
  if (s > 0)
  {    
    for (unsigned int i = 0; i < s; i++)
    {
      m_children[i]->UpdateBoundingVol();
      // Increase our box to fit the child
      m_aabb.Union(*(m_children[i]->GetAABB()));
    }
  }

#ifdef AABB_DEBUG
  // Zero size box? Not good, AABB not set up, culling won't work
  Assert(m_aabb.GetXSize() != 0 || m_aabb.GetYSize() != 0 || m_aabb.GetZSize() != 0);
#endif
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
    f->GetDataLine(&s); 
    PSceneNode p = TheSceneNodeFactory::Instance()->Create(s);
    if (!p)
    {
      f->ReportError("Failed to create scene node of type '" + s + "'");
      Assert(0);
    }

    if (!p->Load(f))
    {
      f->ReportError("Failed to load child of scene node");
      Assert(0);
      return false;
    }

    m_children.push_back(p);
  }
  return true;
}

SceneNode* SceneNode::GetNodeByName(const std::string& name)
{
  if (GetName() == name)
  {
    return this;
  }
  for (auto ch : m_children)
  {
    SceneNode* node = ch->GetNodeByName(name);
    if (node)
    {
      return node;
    }
  }
  return nullptr;
}

bool SceneNode::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected scene node name");
    return false;
  }

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
    m_combined = m_local * m_parent->m_combined;
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
static const uint32 Z_READ     = 0x20;
static const uint32 Z_WRITE    = 0x40;

bool SceneNode::IsZReadEnabled() const
{
  // Flag == 0 means enabled
  return (m_flags & Z_READ) == 0;
}

bool SceneNode::IsZWriteEnabled() const
{
  // Flag == 0 means enabled
  return (m_flags & Z_WRITE) == 0;
}

void SceneNode::SetIsZReadEnabled(bool b)
{
  // Flag == 0 means enabled
  if (b)
  {
    m_flags &= ~Z_READ;
  }
  else
  {
    m_flags |= Z_READ;
  }
}

void SceneNode::SetIsZWriteEnabled(bool b)
{
  // Flag == 0 means enabled
  if (b)
  {
    m_flags &= ~Z_WRITE;
  }
  else
  {
    m_flags |= Z_WRITE;
  }
}

bool SceneNode::IsLit() const
{
  return (m_flags & LIGHTING) != 0;
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
