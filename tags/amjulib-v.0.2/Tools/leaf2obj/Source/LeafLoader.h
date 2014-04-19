#ifndef LEAF_LOADER_H
#define LEAF_LOADER_H

#include <string>

namespace Amju
{
class ObjMesh;

ObjMesh* LeafLoad(const std::string& filename);
}

#endif
