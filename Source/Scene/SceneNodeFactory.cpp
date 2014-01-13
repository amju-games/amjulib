#include "SceneNodeFactory.h"
#include "SceneNode.h"
#include "SceneMesh.h"
#include "SceneLuaScript.h"
// TODO
//#include "Billboard.h"
//#include "ParticleEffect2d.h"
//#include "SceneNodeCamera.h"
//#include "Shadow.h"

namespace Amju
{
SceneNodeFactory::SceneNodeFactory()
{
  // Add SceneNode types to factory
  TheSceneNodeFactory::Instance()->Add(SceneNode::NAME, &SceneNode::Create);
  TheSceneNodeFactory::Instance()->Add(SceneMesh::NAME, &SceneMesh::Create);
  TheSceneNodeFactory::Instance()->Add(SceneLuaScript::NAME, &SceneLuaScript::Create);
}
}
