#include "ObjMesh.h"
#include "AmjuGL-Null.h"
#include <iostream>

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../../../Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../Build/Release/AmjuLibMsvc.lib")
#endif // _DEBUG
#endif // WIN32

using namespace Amju;

namespace Amju
{
  void ReportError(const std::string& msg)
  {
    std::cout << msg << "\n";
  }
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cout << "Obj2Bin converts .obj meshes to binary format\nUsage: obj2bin <input file> <output file>\n";
    return 1;
  }

  ObjMesh::SetShowInfo(true);
  ObjMesh::SetRequireTextures(false);
  AmjuGL::SetImpl(new Amju::AmjuGLNull);

  // Resource manager was changed so we don't have to create a group for
  //  every obj we want to load.
//	TheResourceManager::Instance()->LoadResourceGroup("crate2-group");

  std::string inFilename = argv[1]; 
  std::string outFilename = argv[2]; 

  if (inFilename == outFilename)
  {
    std::cout << "Not allowed to overwrite original data file.\n";
    return 1;
  }

  ObjMesh mesh;
  if (!mesh.Load(inFilename, false /* not bin */))
  {
    std::cout << "Failed to load OBJ: " << inFilename << "\n";
    return -1;
  }

  std::cout << "----------------------------------------------------------\nSaving " << 
    inFilename << " to binary file " << outFilename << "...\n";

  if (mesh.Save(outFilename, true /* binary */))
  {
    std::cout << "Saved output file " << outFilename << ".. now testing it by loading it in...\n";
    ObjMesh mesh2;
    if (mesh2.Load(outFilename, true /* bin */))
    {
      std::cout << ".. Probably converted OK\n";
    }
    else
    {
      std::cout << ".. Convert failed :-(\n";
    }
  }
  else
  {
    std::cout << "Failed to save output file " << outFilename << "\n";
  }

  std::cout << "==========================================================\n";
  return 0;
}
