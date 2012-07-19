#ifndef LOADER_H
#define LOADER_H

#include <string>

namespace Amju
{
class ObjMesh;

// Load .comp or .leaf
ObjMesh* Load(const std::string& fileName);
}

#endif

