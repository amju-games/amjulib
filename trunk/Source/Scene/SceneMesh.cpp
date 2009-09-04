#include "SceneMesh.h"
#include "File.h"
#include "AmjuGL.h"
#include "CollisionMesh.h"

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
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  m_mesh->Draw();
  AmjuGL::PopMatrix();

  GetAABB()->Draw();
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

void SceneMesh::UpdateBoundingVol()
{
  // Update bounding vol from mesh
  // TODO This is slow, it works out the bounding box for the mesh given its
  //  combined transform. Better to pass in an AABB if you already know it
  //  (e.g. GameObjects have a bounding volume)
  CollisionMesh cm;
  m_mesh->CalcCollisionMesh(&cm);
  cm.Transform(m_combined);
  cm.CalcAABB(&m_aabb);

  if (m_parent)
  {
    m_parent->UpdateBoundingVol();
  }
}
}
