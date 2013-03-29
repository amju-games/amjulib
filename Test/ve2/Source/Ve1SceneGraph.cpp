#include <AmjuFirst.h>
#include "Ve1SceneGraph.h"
#include "Camera.h"
#include "Cam2d.h"
#include "Useful.h"
#include "GameMode.h"
#include <AmjuFinal.h>

#define CAM_2D

namespace Amju
{
bool SpriteBlendNodeComp(
  const SceneGraph::BlendNode& b1, const SceneGraph::BlendNode& b2)
{
  /*
  if (b1.m_pos.y < b2.m_pos.y)
  {
    return true;
  }
  else if (b1.m_pos.y > b2.m_pos.y)
  {
    return false;
  }
  */

  // Y values equal
  if (b1.m_pos.z < b2.m_pos.z)
  {
    return true;
  }

  return false;
}

void ClearVe1SceneGraph()
{
  SceneGraph* sg = GetVe1SceneGraph();
  sg->Clear();
  sg->SetRootNode(SceneGraph::AMJU_OPAQUE, new SceneNode);

#ifdef CAM_2D
  if (GetGameMode() == AMJU_MODE_EDIT)
  {
    sg->SetCamera(new Camera);
  }
  else
  {
    sg->SetCamera(new Cam2d);
  }
#else
  sg->SetCamera(new Camera);
#endif
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
    sg->SetBlendNodeComp(SpriteBlendNodeComp);
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

