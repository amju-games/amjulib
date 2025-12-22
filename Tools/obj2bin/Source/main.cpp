#include <iostream>
#include "ObjMesh.h"
#include "AmjuGL-Null.h"

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

  std::string inFilename = argv[1]; 
  std::string outFilename = argv[2]; 

  if (inFilename == outFilename)
  {
    std::cout << "Not allowed to overwrite original data file.\n";
    return 1;
  }

  ObjMesh mesh;
  mesh.SetIsBinary(false);
  if (!mesh.Load(inFilename))
  {
    std::cout << "Failed to load OBJ: " << inFilename << "\n";
    return -1;
  }

  std::cout << "----------------------------------------------------------\nSaving " << 
    inFilename << " to binary file " << outFilename << "...\n";

  mesh.SetIsBinary(true);
  if (mesh.Save(outFilename))
  {
    std::cout << "Saved output file " << outFilename << ".. now testing it by loading it in...\n";
    ObjMesh mesh2;
    mesh2.SetIsBinary(true);
    if (mesh2.Load(outFilename))
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
