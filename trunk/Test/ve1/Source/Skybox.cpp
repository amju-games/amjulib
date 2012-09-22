#include "Skybox.h"
#include "GameObjectFactory.h"
#include "Timer.h"
#include "LoadMeshResource.h"
#include "SceneMesh.h"
#include "Ve1SceneGraph.h"
#include "SetObjMeshCommand.h"
#include "SceneCollisionMesh.h"

namespace Amju
{
GameObject* CreateSkybox() { return new Skybox; }
static bool reg = TheGameObjectFactory::Instance()->Add(Skybox::TYPENAME, &CreateSkybox);

const char* Skybox::TYPENAME = "skybox";

Skybox::Skybox()
{
  m_xRot = 0;
  m_yRot = 0;
}

const char* Skybox::GetTypeName() const
{
  return TYPENAME;
}

void Skybox::SetEditMenu(GuiMenu* menu)
{
  menu->AddChild(new GuiMenuItem("Set obj mesh...", new SetObjMeshCommand(GetId())));

  // TODO
}

bool Skybox::Load(File* f)
{
  // Get mesh filename.
  std::string meshFilename;
  if (!f->GetDataLine(&meshFilename))
  {
    f->ReportError("Expected mesh file name for skybox");
    return false;
  }

  // Obj mesh comes last, as mesh file name is appended to data file contents when we upload
  ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(meshFilename);
  if (!mesh)
  {
    return false;
  }

  Assert(mesh);
  SceneCollisionMesh* sm = new SceneCollisionMesh;
  sm->SetMesh(mesh);
  sm->CalcCollisionMesh();
  sm->GetCollisionMesh()->CalcAABB(&m_aabb);
  *(sm->GetAABB()) = m_aabb;

  SetSceneNode(sm);

#ifdef AABB_DEBUG
std::cout << "Got AABB for " << *this << " size: " << m_aabb.GetXSize() << " " << m_aabb.GetYSize() << " " << m_aabb.GetZSize() << "\n";
#endif

  return true;


/*
  if (!GameObject::Load(f))
  {
    return false;
  }

  m_mesh = LoadMeshResource(f);
  Assert(m_mesh);

  SceneMesh* sm = new SceneMesh;
  sm->SetMesh(m_mesh);
  m_sceneNode = sm;
  GetVe1SceneGraph()->SetRootNode(SceneGraph::AMJU_SKYBOX, m_sceneNode);

  return true;
*/
}

/*
void Skybox::Draw()
{
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushMatrix();

  AmjuGL::RotateX(m_xRot);
  AmjuGL::RotateY(m_yRot);
  m_mesh->Draw();

  AmjuGL::PopMatrix();
}
*/

void Skybox::Update()
{
//  static const float ROT_VEL = 2.0f; // degs/dec
//  m_yRot += ROT_VEL * TheTimer::Instance()->GetDt();

  Matrix mat;
  mat.RotateX(m_xRot);
  Matrix mat2;
  mat.RotateY(m_yRot);
  m_sceneNode->SetLocalTransform(mat);
  m_sceneNode->MultLocalTransform(mat2);
}
}
