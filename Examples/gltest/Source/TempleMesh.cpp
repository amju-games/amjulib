#include <File.h>
#include "MySceneGraph.h"
#include "TempleMesh.h"  

namespace Amju
{
const char* TempleMesh::NAME = "templemesh";

const char* TempleMesh::GetTypeName() const 
{
  return NAME;
}

bool TempleMesh::Load(File* f)
{
  if (!GameObject::Load(f))
    return false;

  // Load scene mesh, materials, shaders, calc tangents.
  std::string meshFilename;
  if (!f->GetDataLine(&meshFilename))
  {
    f->ReportError("Expected mesh filename");
    return false;
  }

  // TODO octree node type

  if (!CreateSceneNode(meshFilename))
  {
    f->ReportError("Failed to create scene node for " + meshFilename);
    return false;
  }

  m_sceneNode->SetIsLit(true);
  AddSceneNodeToGraph();

  return true;
}

void TempleMesh::Update() 
{
}

SceneGraph* TempleMesh::GetSceneGraph() 
{
  return Amju::GetSceneGraph();
}

}

