#pragma once

#include <string>

namespace Amju
{
class Squishy;

// Load a squishy from a .obj mesh
bool SquishyLoadObj(Squishy* sq, const std::string& objFile);
}
