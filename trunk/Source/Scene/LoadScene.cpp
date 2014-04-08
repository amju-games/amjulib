#include <StringUtils.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <File.h>
#include <LoadMatrix.h>
#include "LoadScene.h"
#include "SceneMesh.h"
#include "SceneNodeFactory.h"

namespace Amju
{
PSceneNode LoadScene(const std::string& filename)
{
  if (filename.empty())
  {
    return new SceneNode;
  }

  // TODO Factory of loader types
  if (GetFileExt(filename) == "obj")
  {
    ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(filename);
    if (!mesh)
    {
      return nullptr;
    }

    SceneMesh* sm  = new SceneMesh;
    sm->SetMesh(mesh);
    return sm;
  }

  File f;
  if (!f.OpenRead(filename))
  {
    return 0;
  }
  return LoadScene(&f);
}

PSceneNode LoadScene(File* f)
{
  std::string nodeType;
  if (!f->GetDataLine(&nodeType))
  {
    f->ReportError("Expected scene node type or .obj filename");
    return 0;
  }

  PSceneNode node;

  std::string ext = GetFileExt(nodeType);
  if (ext == "obj")
  {
    // We got a .obj filename -- load mesh and create SceneMesh node
    ObjMesh* mesh = (ObjMesh*)TheResourceManager::Instance()->GetRes(nodeType);
    if (!mesh) 
    {
      return 0;
    }

    SceneMesh* sm  = new SceneMesh;
    sm->SetMesh(mesh);

    node = sm;
  }
  else
  {
    // Another type of node -- create and load
    node = TheSceneNodeFactory::Instance()->Create(nodeType);
    if (!node)
    {
      f->ReportError("Failed to create scene node of type: " + nodeType);
      return 0;
    }
    if (!node->Load(f))
    {
      return 0;
    }
  }

  // Load orientation etc
  Matrix m;
  if (!LoadMatrix(f, &m))
  {
    f->ReportError("Failed to load transform matrix for node " + nodeType);
    return 0;
  }
  node->SetLocalTransform(m);

  int numChildren = 0;
  if (!f->GetInteger(&numChildren))
  {
    // OK, we are not specifying any children - allow this.
    //f->ReportError("Expected num children for scene node.");
    return node;
  }
  for (int i = 0; i < numChildren; i++)
  {
    PSceneNode child = LoadScene(f);
    if (!child)
    {
      return 0;
    }
    node->AddChild(child);
  }
  return node;
}

}

