#include <StringUtils.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <File.h>
#include "LoadScene.h"
#include "SceneMesh.h"
#include "SceneNodeFactory.h"

namespace Amju
{
PSceneNode LoadScene(const std::string& filename)
{
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
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected scene node type");
    return 0;
  }
  PSceneNode node = TheSceneNodeFactory::Instance()->Create(s);
  if (!node)
  {
    f->ReportError("Failed to create scene node of type: " + s);
    return 0;
  }
  if (!node->Load(f))
  {
    return 0;
  }
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

