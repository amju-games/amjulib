#include "Ve1SceneGraph.h"
#include "Camera.h"

namespace Amju
{
void ClearVe1SceneGraph()
{
  SceneGraph* sg = GetVe1SceneGraph();
  sg->Clear();
  sg->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  sg->SetCamera(new Camera);
}

void ClearGuiSceneGraph()
{
  SceneGraph* sg = GetGuiSceneGraph();
  sg->Clear();
  sg->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  //sg->SetCamera(new Camera);
}

SceneGraph* GetVe1SceneGraph()
{
  static SceneGraph* sg = 0;
  if (!sg)
  {
    sg = new SceneGraph;
  }
  return sg;
}

SceneGraph* GetGuiSceneGraph()
{
  static SceneGraph* sg = 0;
  if (!sg)
  {
    sg = new SceneGraph;
    sg->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  } 
  return sg;
}

}

