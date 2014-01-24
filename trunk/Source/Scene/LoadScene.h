#pragma once

#include "SceneNode.h"

namespace Amju
{
// Create scene node based on file extension
PSceneNode LoadScene(const std::string& filename);

// Create scene from description in the given text file
PSceneNode LoadScene(File* f);
}

