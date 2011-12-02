#include "Ve1SceneGraph.h"

namespace Amju
{
SceneGraph* GetVe1SceneGraph()
{
  static SceneGraph* sg = new SceneGraph;
  return sg;
}

SceneGraph* GetGuiSceneGraph()
{
  static SceneGraph* sg = new SceneGraph;
  return sg;
}

}

