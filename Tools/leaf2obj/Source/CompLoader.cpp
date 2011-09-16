#include "CompLoader.h"
#include "Loader.h"
#include <ObjMesh.h>
#include <File.h>
#include <DegRad.h>
#include <Matrix.h>

namespace Amju
{
ObjMesh* CompLoad(const std::string& compFile)
{
  File f;
  if (!f.OpenRead(compFile))
  {
    return 0;
  }

  // Format is:
  // num components
  // component name (.leaf or .comp)
  // x, y, z translation
  // x, y, z rotation

  // NB Recursive/tree structure
  
  int numComps = 0;
  if (!f.GetInteger(&numComps))
  {
    f.ReportError("Expected num components");
    return 0;
  }
 
  ObjMesh* parent = 0;
  for (int i = 0; i < numComps; i++)
  {
    std::string compName;
    if (!f.GetDataLine(&compName))
    {
      f.ReportError("Expected component file name");
      return 0;
    }

    // Load the component, giving an ObjMesh.
    ObjMesh* mesh = Load(compName); 
    if (!mesh)
    {
      f.ReportError("Failed to load " + compName);
      return 0;
    }
    float x, y, z, rx, ry, rz;
    if (!f.GetFloat(&x) || !f.GetFloat(&y) || !f.GetFloat(&z) || 
        !f.GetFloat(&rx) || !f.GetFloat(&ry) || !f.GetFloat(&rz))
    {
      f.ReportError("Failed to load transform for " + compName);
      return 0;
    }

    // Create transformation matrix, transform mesh
    Matrix mat;
    mat.SetIdentity();
    mat.Translate(Vec3f(x, y, z));
    // Rotations -- correct order ??
    Matrix matR;
    matR.RotateX(DegToRad(rx));
    mat *= matR;
    matR.RotateY(DegToRad(ry));
    mat *= matR;
    matR.RotateZ(DegToRad(rz));
    mat *= matR;

    mesh->Transform(mat);

    if (parent)
    {  
      parent->Merge(*mesh);
    }
    else
    {
      parent = mesh;
    }
  }

  parent->Save(compFile + ".obj");
  return parent;
}
}

