#include "MySceneGraph.h"

namespace Amju
{
SceneGraph* GetSceneGraph()
{
  static SceneGraph sg;
  return &sg;
}

}

