#include <AmjuFirst.h>
#include "SceneMesh.h"
#include "File.h"
#include "AmjuGL.h"
#include "CollisionMesh.h"
#include "DrawAABB.h"
#include <AmjuFinal.h>

//#define DEBUG_DRAW_AABB

namespace Amju
{
const char* SceneMesh::NAME = "mesh";

SceneNode* SceneMesh::Create() { return new SceneMesh; }

void SceneMesh::SetMesh(PObjMesh mesh)
{
  m_mesh = mesh;
}

void SceneMesh::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  if (IsLit())
  {
    AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  }
  else
  {
    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  }

  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  m_mesh->Draw();
  AmjuGL::PopMatrix();

  AmjuGL::PopAttrib();
}

void SceneMesh::Update()
{
}

bool SceneMesh::Load(File* f)
{
  if (!SceneNode::LoadMatrix(f))
  {
    return false;
  }
  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  m_mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(s);
  if (!m_mesh)
  {
    return false;
  }

  // TODO Should a mesh node be able to have children ? 
  // I guess so..
  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
}
}
