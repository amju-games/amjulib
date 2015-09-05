#include "MySceneGraph.h"

namespace Amju
{
SceneGraph* GetSceneGraph()
{
  static SceneGraph sg;
  return &sg;
}

void ResetSceneGraph()
{
  GetSceneGraph()->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);
  GetSceneGraph()->SetCamera(new SceneNodeCamera);
}

}

