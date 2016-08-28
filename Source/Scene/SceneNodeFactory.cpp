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
  Add(SceneNode::NAME, &SceneNode::Create);
  Add(SceneMesh::NAME, &SceneMesh::Create);
  Add(SceneMeshMaterial::NAME, []() -> SceneNode* { return new SceneMeshMaterial;} );
  Add(SceneLuaScript::NAME, &SceneLuaScript::Create);
}
}
