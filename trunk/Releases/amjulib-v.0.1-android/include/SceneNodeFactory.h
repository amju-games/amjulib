#pragma once

#include <Factory.h>
#include <Singleton.h>
#include "SceneNode.h"

namespace Amju
{
class SceneNodeFactory : public Factory<SceneNode>, public NonCopyable
{
public:
  SceneNodeFactory();
};

typedef Singleton<SceneNodeFactory> TheSceneNodeFactory;
}
