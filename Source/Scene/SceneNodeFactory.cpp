#include "SceneLuaScript.h"
#include "SceneMesh.h"
#include "SceneNode.h"
#include "SceneNodeCamera.h"
#include "SceneNodeFactory.h"
// TODO
//#include "Billboard.h"
//#include "ParticleEffect2d.h"
//#include "Shadow.h"

namespace Amju
{
SceneNodeFactory::SceneNodeFactory()
{
  // Add SceneNode types to factory
  Add(SceneNode::NAME, &SceneNode::Create);
  Add(SceneMesh::NAME, &SceneMesh::Create);
  Add(SceneMeshMaterial::NAME, []() -> SceneNode* { return new SceneMeshMaterial;} );
  Add(SceneNodeCamera::NAME, []() -> SceneNode* { return new SceneNodeCamera; });
  Add(SceneLuaScript::NAME, &SceneLuaScript::Create);
}
}
