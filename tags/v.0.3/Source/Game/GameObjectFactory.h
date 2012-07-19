#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObject.h"
#include "Factory.h"
#include "Singleton.h"

namespace Amju
{
typedef Singleton<Factory<GameObject> > TheGameObjectFactory;
}

#endif
