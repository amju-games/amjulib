#include <GameObjectFactory.h>
#include <File.h>
#include <Texture.h>
#include "Baddie.h"
#include "Ve1SceneGraph.h"
#include "Ve1Character.h"

namespace Amju
{
GameObject* CreateBaddie()
{
  return new Baddie;
}

static bool registered = TheGameObjectFactory::Instance()->Add(Baddie::TYPENAME, CreateBaddie);


const char* Baddie::TYPENAME = "baddie";

const char* Baddie::GetTypeName() const
{
  return TYPENAME;
}

void Baddie::Update()
{
  Ve1ObjectChar::Update();
}

bool Baddie::Load(File* f)
{
  // Create Scene Node, but don't attach to SceneGraph until needed
  Ve1Character* node = new Ve1Character;

  m_sceneNode = node;

  m_shadow = new Shadow;
  if (!m_shadow->Load(f))
  {
    return false;
  }
  m_sceneNode->AddChild(m_shadow.GetPtr());

  m_inNewLocation = true;

  return true;
}

}


