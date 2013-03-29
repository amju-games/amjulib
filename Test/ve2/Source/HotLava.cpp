#include <AmjuFirst.h>
#include <GameObjectFactory.h>
#include "HotLava.h"
#include <AmjuFinal.h>

namespace Amju
{
HotLava::HotLava()
{
  m_isPickable = false;
}

const char* HotLava::TYPENAME = "hotlava";

const char* HotLava::GetTypeName() const
{
  return TYPENAME;
}

GameObject* CreateHotLava()
{
  return new HotLava;
}

static bool registered = TheGameObjectFactory::Instance()->Add(HotLava::TYPENAME, CreateHotLava);

void HotLava::SetEditMenu(GuiMenu* menu)
{
}

//void HotLava::Update()
//{
//}

}
