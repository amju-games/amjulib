#ifndef COMP_LOADER_H
#define COMP_LOADER_H

#include <string>

namespace Amju
{
class ObjMesh;

ObjMesh* CompLoad(const std::string& compFile);
}

#endif

