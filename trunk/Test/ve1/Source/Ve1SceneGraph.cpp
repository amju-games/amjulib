#include "Ve1SceneGraph.h"

namespace Amju
{
SceneGraph* GetVe1SceneGraph()
{
  static SceneGraph* sg = 0;
  if (!sg)
  {
    sg = new SceneGraph;

    sg->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  }
  return sg;
}

SceneGraph* GetGuiSceneGraph()
{
  static SceneGraph* sg = new SceneGraph;
  return sg;
}

}

