#include <GameObjectFactory.h>
#include "FireTexture.h"
#include "Init.h"
#include <ObjMesh.h>
#include "TempleMesh.h"
#include <ResourceManager.h>

namespace Amju
{

template<class T>
GameObject* Create() { return new T; } 

void Init()
{
  auto* gof = TheGameObjectFactory::Instance();
  
  gof->Add(FireTexture::NAME, &Create<FireTexture>);
  gof->Add(Fire::NAME, &Create<Fire>);
  gof->Add(TempleMesh::NAME, &Create<TempleMesh>);

  // Set up any unusual resource loaders
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("obj", TextObjLoader);

  // TODO collision double dispatcher?

}
}
