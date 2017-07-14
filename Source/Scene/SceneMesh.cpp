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
  m_aabb = mesh->GetAABB();
  CombineTransform();
  m_aabb.Transform(GetCombinedTransform());
  SetShowAABB(true);
}

const ObjMesh* SceneMesh::GetMesh() const
{
  return m_mesh;
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
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected scene node name");
    return false;
  }

  if (!SceneNode::LoadMatrix(f))
  {
    return false;
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    return false;
  }
  PObjMesh mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(s);
  if (!mesh)
  {
    return false;
  }
  SetMesh(mesh);

  // TODO Should a mesh node be able to have children ? 
  // I guess so..
  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
}
  
void SceneMesh::CalcCollisionMesh(CollisionMesh* pCollMesh) const
{
  Assert(m_mesh);
  Assert(pCollMesh);
  m_mesh->CalcCollisionMesh(pCollMesh);
}

 
// TODO Separate file
const char* SceneMeshMaterial::NAME = "scene-mesh-material";
    
bool SceneMeshMaterial::Load(File* f)
{
  // Load mesh name, then material name
  std::string meshFileName;
  if (!f->GetDataLine(&meshFileName))
  {
    f->ReportError("Expected .obj filename");
    return false;
  }
  
  m_mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(meshFileName);
  if (!m_mesh)
  {
    return false;
  }
  
  // Material
  std::string materialFileName;
  if (!f->GetDataLine(&materialFileName))
  {
    f->ReportError("Expected material filename");
    return false;
  }
  
  MaterialVec matvec;
  if (!LoadMtlFile(materialFileName, &matvec))
  {
    f->ReportError("Failed to load material filename: " + materialFileName);
    return false;
  }
  
  if (matvec.empty())
  {
    f->ReportError("No materials in file: " + materialFileName);
    return false;
  }
  
  // Should be only one material
  SetMaterial(*matvec[0]);
  
  return true;
}

void SceneMeshMaterial::Draw()
{
  // This will work best for meshes with a single group and no material specified.
  m_material.UseThisMaterial();
  SceneMesh::Draw();
}
  
void SceneMeshMaterial::SetMaterial(const Material& m)
{
  m_material = m;
}
}
