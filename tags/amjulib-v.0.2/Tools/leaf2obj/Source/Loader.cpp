#include "Loader.h"
#include "LeafLoader.h"
#include "CompLoader.h"

namespace Amju
{
ObjMesh* Load(const std::string& filename)
{
  if (filename.find(".comp") != std::string::npos)
  {
    return CompLoad(filename);
  }
  else if (filename.find(".leaf") != std::string::npos)
  {
    return LeafLoad(filename);
  }
  else
  {
    return 0;
  }
}
}

